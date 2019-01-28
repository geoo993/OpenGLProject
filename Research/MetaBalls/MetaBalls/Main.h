#include "../Framework2/OpenGL/OpenGLApp.h"
#include "../Framework2/Util/IsoSurface.h"

#define BALL_COUNT 4
#define RADIUS 30.0f

#ifdef _WIN32
__declspec(align(16))
#endif
struct MetaBall {
	vec3 pos;
	float padd;
};

struct MetaBall2 {
	vec3 pos;
	vec3 dir;
};

class MainApp : public OpenGLApp {
public:
	void resetCamera();

	void initMenu();
	bool init();
	bool exit();

	bool load();
	bool unload();

	void drawEnvironment();
	bool drawFrame();
protected:
	IsoSurface *metaBalls;

	MetaBall2 mBalls[BALL_COUNT];

	ShaderID shader;
	TextureID env;
	int method, mFPU, m3DNow, mSSE, mSSE3;
	int nThreads;
	bool showStats;
};
