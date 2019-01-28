#include "Main.h"
#include "../Framework2/CPU.h"

void MainApp::resetCamera(){
	position = vec3(200, 0, 120);
	wx = 0;
	wy = 2.15f;
	wz = 0;
}

void MainApp::initMenu(){
	method = 0;
	mFPU   = 0;
	m3DNow = -1;
	mSSE   = -1;
	mSSE3  = -1;

	nThreads = 1;

	showStats = false;

	Menu *menu = menuSystem->getMainMenu();
#ifdef _WIN32

	SYSTEM_INFO sysInfo;
	GetSystemInfo(&sysInfo);
	nThreads = min(sysInfo.dwNumberOfProcessors, 4);

	static char *options[4] = { "FPU" };

	if (cpuSSE){
		method++;
		options[mSSE = method] = "SSE";
	}
#if _MSC_VER >= 1400
	if (cpuSSE3){
		method++;
		options[mSSE3 = method] = "SSE3";
	}
#endif
	if (cpu3DNow){
		method++;
		options[m3DNow = method] = "3Dnow";
	}

	menu->addMenuItem("Compute with: ", &method, INPUT_INTEGER, 0, method, options);

	menu->addMenuItem("Number of threads: ", &nThreads, INPUT_INTEGER, 1, 4);
#endif
	menu->addMenuItem("Show stats: ", &showStats, INPUT_BOOL);
	App::initMenu();
}

bool MainApp::init(){
	metaBalls = new IsoSurface(4);
	metaBalls->setSize(vec3(-150, -150, -150), vec3(150, 150, 150), 40, 40, 40);

	mBalls[0].pos = vec3(-70, 10, 0);
	mBalls[1].pos = vec3( 70, 0, 10);
	mBalls[2].pos = vec3( 10,  80, 50);
	mBalls[3].pos = vec3(-10, -80, 100);

	for (unsigned int i = 0; i < BALL_COUNT; i++){
		mBalls[i].dir = vec3(0, 0, 0);
	}

	return true;
}

bool MainApp::exit(){
	delete metaBalls;
	return true;
}

bool MainApp::load(){
	if (!GL_ARB_shader_objects_supported || !GL_ARB_vertex_shader_supported || !GL_ARB_fragment_shader_supported || !GL_ARB_shading_language_100_supported){
		addToLog("No GLSL support (GL_ARB_shader_objects, GL_ARB_vertex_shader, GL_ARB_fragment_shader, GL_ARB_shading_language_100)\n");
		return false;
	}

	setDefaultFont("../Textures/Fonts/Future.font", "../Textures/Fonts/Future.dds");

	if ((shader = renderer->addShader("shader.shd")) == SHADER_NONE) return false;

	if ((env = renderer->addCubemap(
		"../Textures/CubeMaps/Glacier2/negx.jpg",
		"../Textures/CubeMaps/Glacier2/posx.jpg",
		"../Textures/CubeMaps/Glacier2/negy.jpg",
		"../Textures/CubeMaps/Glacier2/posy.jpg",
		"../Textures/CubeMaps/Glacier2/negz.jpg",
		"../Textures/CubeMaps/Glacier2/posz.jpg")) == TEXTURE_NONE) return false;

	return true;
}

bool MainApp::unload(){
	return true;
}

void MainApp::drawEnvironment(){
	static float vertices[] = { -5, 5, 5, 5, 5, 5, -5, -5, 5, 5, -5, 5, -5, 5, -5, 5, 5, -5, -5, -5, -5, 5, -5, -5 };
	static short indices[] = { 0,1,2,  1,3,2,  1,5,3,  5,7,3,  5,4,7,  4,6,7,  4,0,6,  0,2,6,  4,5,0,  5,1,0,  2,3,6,  3,7,6 };

	renderer->setTextures(env);
	renderer->setDepthFunc(DEPTH_NONE);
	renderer->apply();

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glVertexPointer  (3, GL_FLOAT, 3 * sizeof(float), vertices);
	glTexCoordPointer(3, GL_FLOAT, 3 * sizeof(float), vertices);

	glColor3f(1, 1, 1);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, indices);

	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

static MetaBall balls[BALL_COUNT];

#ifdef _WIN32
float field_3DNow(const vec3 &pos){
	float r = -1.0f;
	const float invRadius = 1.0f / RADIUS;

	__asm {
		femms

		lea eax, [balls]
 		mov ebx, pos
		movq mm2, [ebx]     // [pos.xy]
		movd mm3, [ebx + 8] // [pos.z, 0]

		movd mm5, invRadius   // [invRadius, 0]
		pshufw mm5, mm5, 0x44 // [invRadius, invRadius]
		pfmul mm2, mm5        // [ball.pos.xy / RADIUS]
		pfmul mm3, mm5        // [ball.pos.z  / RADIUS, 0]

		movd mm4, r
		mov ecx, BALL_COUNT

		start:
			movq mm0, [eax]     // [ball.pos.xy / RADIUS]
			movq mm1, [eax + 8] // [ball.pos.z  / RADIUS, 0]

			pfsub mm0, mm2      // [d.xy / RADIUS]
			pfsub mm1, mm3      // [d.z / RADIUS, 0]

			pfmul mm0, mm0      // [d.x * d.x / (RADIUS * RADIUS), d.y * d.y / (RADIUS * RADIUS)]
			pfmul mm1, mm1      // [d.z * d.z / (RADIUS * RADIUS), 0]

			pfacc mm0, mm0      // [(d.x * d.x + d.y * d.y) / (RADIUS * RADIUS), --]
			pfadd mm0, mm1      // [dot(d, d) / (RADIUS * RADIUS), --]
			pfrcp mm0, mm0      // [RADIUS * RADIUS / dot(d, d), --]

			pfadd mm4, mm0      // Accumulate results

			add eax, 16
			dec ecx
		jnz start

		movd r, mm4

		femms
    }

	return r;
}

float field_SSE(const vec3 &pos){
	float r = -1.0f;
	const float invRadius = 1.0f / RADIUS;

	__asm {
		lea eax, [balls]
		mov ebx, pos

		movss   xmm1, [ebx + 8] // [pos.z, 0, 0, 0]
		movlhps xmm1, xmm1      // [pos.z, 0, pos.z, 0]
		movlps  xmm1, [ebx]     // [pos.xyz, 0]

		movss  xmm3, invRadius
		shufps xmm3, xmm3, 0x0
		mulps  xmm1, xmm3

		movss xmm2, r
		mov ecx, BALL_COUNT

		start:
			movaps xmm0, [eax]      // [ball.pos / RADIUS, 0]
			subps  xmm0, xmm1       // [d / RADIUS, 0]
			mulps  xmm0, xmm0       // [d * d / (RADIUS * RADIUS), 0]

			movaps xmm3, xmm0       // [d * d / (RADIUS * RADIUS), 0]
			shufps xmm3, xmm3, 0x39 // [d.yz * d.yz / (RADIUS * RADIUS, --, --]

			addss  xmm0, xmm3       // [(d.x * d.x + d.y * d.y) / (RADIUS * RADIUS, --, --, --]
			shufps xmm3, xmm3, 0x39 // [d.z * d.z / (RADIUS * RADIUS, --, --, --]
			addss  xmm0, xmm3       // [dot(d, d / (RADIUS * RADIUS), --, --, --]

			rcpss xmm0, xmm0        // [RADIUS * RADIUS / dot(d, d), --, --, --]

			addss xmm2, xmm0        // Accumulate results

			add eax, 16
			dec ecx
		jnz start

		movss r, xmm2
	}

	return r;
}

#if _MSC_VER >= 1400

float field_SSE3(const vec3 &pos){
	float r = -1.0f;
	const float invRadius = 1.0f / RADIUS;

	__asm {
		lea eax, [balls]
		mov ebx, pos

		movss   xmm1, [ebx + 8] // [pos.z, 0, 0, 0]
		movlhps xmm1, xmm1      // [pos.z, 0, pos.z, 0]
		movlps  xmm1, [ebx]     // [pos.xyz, 0]

		movss  xmm3, invRadius
		shufps xmm3, xmm3, 0x0
		mulps  xmm1, xmm3

		movss xmm2, r
		mov ecx, BALL_COUNT

		start:
			movaps xmm0, [eax]      // [ball.pos / RADIUS, 0]
			subps  xmm0, xmm1       // [d / RADIUS, 0]
			mulps  xmm0, xmm0       // [d * d / (RADIUS * RADIUS), 0]

			haddps xmm0, xmm0
			haddps xmm0, xmm0       // [dot(d, d) / (RADIUS * RADIUS)]
			rcpss  xmm0, xmm0       // [RADIUS * RADIUS / dot(d, d), --, --, -- ]

			addss  xmm2, xmm0       // Accumulate results

			add eax, 16
			dec ecx
		jnz start

		movss r, xmm2
	}

	return r;
}
#endif

#endif

float field_C(const vec3 &pos){
/*
	float sum = -1.0f;
	const vec3 p = pos * (1.0f / RADIUS);
	for (unsigned int i = 0; i < BALL_COUNT; i++){
		vec3 d = balls[i].pos - p;
		sum += 1.0f / dot(d, d);
	}
	return sum;
*/
	float sum = 0.0f;
	float prod = 1.0f;

	const vec3 p = pos * (1.0f / RADIUS);
	for (unsigned int i = 0; i < BALL_COUNT; i++){
		vec3 d = balls[i].pos - p;
		float b = dot(d, d);

		// Refactorize so that we only need a single divide in the end
		sum *= b;
		sum += prod;
		prod *= b;
	}
	return sum / prod - 1.0f;
}

bool MainApp::drawFrame(){
	glEnable(GL_CULL_FACE);

	renderer->changeMask(ALL);
	glClear(GL_DEPTH_BUFFER_BIT);


	glMatrixMode(GL_PROJECTION);
	projection = projectionMatrixX(1.5f, float(height) / float(width), 1, 2000);
	glLoadMatrixf(transpose(projection));

	modelView = rotateZXY(-wx, -wy, -wz);
    glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(transpose(modelView));

	drawEnvironment();

	modelView.translate(-position);
	glLoadMatrixf(transpose(modelView));


	float ft = min(frameTime, 0.1f);

	unsigned int i, j;
	for (i = 0; i < BALL_COUNT; i++){
		vec3 d = -mBalls[i].pos;
		mBalls[i].dir += d * (40000 * ft / (10000.0f + dot(d, d)));
		for (j = 0; j < BALL_COUNT; j++){
			if (j != i){
				vec3 d = mBalls[j].pos - mBalls[i].pos;
				mBalls[i].dir += d * (20000 * ft / (10000.0f + dot(d, d)));
			}
		}
	}

	for (i = 0; i < BALL_COUNT; i++){
		mBalls[i].pos += mBalls[i].dir * ft;
	}

	for (i = 0; i < BALL_COUNT; i++){
		balls[i].pos = mBalls[i].pos / RADIUS;
	}

#ifdef _WIN32
	if (method == mSSE){
		metaBalls->update(field_SSE, nThreads);
	}
#if _MSC_VER >= 1400
	else if (method == mSSE3){
		metaBalls->update(field_SSE3, nThreads);
	}
#endif
	else if (method == m3DNow){
		metaBalls->update(field_3DNow, nThreads);
	} else
#endif
	{
		metaBalls->update(field_C, nThreads);
	}

	renderer->setShader(shader);
	renderer->setTexture("Env", env);
	renderer->apply();

	renderer->changeShaderConstant3f("camPos", position);




	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);

	char *vertexArray = (char *) metaBalls->getVertexArray();
	glVertexPointer(3, GL_FLOAT, sizeof(ISVertex), vertexArray);
	glNormalPointer(   GL_FLOAT, sizeof(ISVertex), vertexArray + sizeof(vec3));

	glDrawElements(GL_TRIANGLES, metaBalls->getIndexCount(), GL_UNSIGNED_INT, metaBalls->getIndexArray());

	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);


	if (showStats){
		char str[256];
		sprintf(str, "Vertices: %d\nIndices: %d", metaBalls->getVertexCount(), metaBalls->getIndexCount());

		renderer->setTextures(textTexture);
		renderer->setDepthFunc(DEPTH_NONE);
		renderer->setBlending(SRC_ALPHA, ONE_MINUS_SRC_ALPHA);
		renderer->apply();

		drawText(str, 0.01f, 0.88f, 0.04f, 0.06f);
	}
	return true;
}

App *app = new MainApp();
