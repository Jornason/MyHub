#ifndef _MATERIAL_H_
#define _MATERIAL_H_

class Material
{
private:
	float ambient[3];
	float diffuse[3];
	float specular[3];
	float shininess;
    float absorption,reflection,transparency,refractive_index;

public:
	Material(void);
	~Material(void);

	void setAmbient(float r,float g,float b);
	void setDiffuse(float r,float g,float b);
	void setSpecular(float r,float g,float b);
	void setShininess(float r);
    void setAbsorption(float a);
    void setReflection(float r);
    void setTransparency(float t);
    void setRefractiveIndex(float r);

    void init();
	
	float *getAmbient();
	float *getDiffuse();
	float *getSpecular();
	float *getShininess();

    float getAbsorption();
    float getReflection();
    float getTransparency();
    float getRefractiveIndex();
};

#endif
