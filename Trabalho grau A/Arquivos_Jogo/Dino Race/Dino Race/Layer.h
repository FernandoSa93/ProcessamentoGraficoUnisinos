class Layer
{
	int width = 0;
	int height = 0;
	float tx = 0.0f;
	float ty = 0.0f;
	float tz = 0.0f;
	unsigned char *pixels;
	float scrollRateX = 0.0f;
	float scrollRateY = 0.0f;
	int tid = 0;

public:
	Layer();

	Layer carregaCamada(const char* imagem, Layer layer, int idTextura, float z, float scrollRateX);
	Layer carregaCamadaAlpha(const char* imagem, Layer layer, int idTextura, float z, float scrollRateX);

	int getWidth();
	int getHeight();
	float getTX();
	float getTY();
	float getTZ();
	unsigned char *getPixels();
	int getTID();
	float getScrollRateX();

	void setWidth(int input_width);
	void setHeight(int input_height);
	void setTX(float input_tx);
	void setTY(float input_ty);
	void setTZ(float input_tz);
	void setPixels(unsigned char *input_pixels);
	void setTID(int input_tid);
	void setScrollRateX(float input_RateX);
};

