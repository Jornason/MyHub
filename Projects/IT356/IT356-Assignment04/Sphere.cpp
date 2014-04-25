#include "Sphere.h"
Sphere::Sphere(string name)
    :Object(name)
{
	float theta,phi;
    int STACKS=360;
	int i,j;
    int SLICES = 360;
    float PI = 3.14159f;
	float cosphi,sinphi,costheta,sintheta;
    VertexAttribs v;
	for (i=0;i<=STACKS;i++)
	{
        phi = -PI/2 + i*PI/STACKS;
        cosphi = cos(phi);
        sinphi = sin(phi);
		for (j=0;j<=SLICES;j++)
		{
            theta = 2*j*PI/SLICES;
            costheta = cos(theta);
            sintheta = sin(theta);
            v.position[0] = cosphi*costheta;
            v.position[1] = sinphi;
            v.position[2] = -cosphi*sintheta;
            v.position[3] = 1;
            v.normal[0] = cosphi*costheta;
            v.normal[1] = sinphi;
            v.normal[2] = -cosphi*sintheta;
            v.normal[3] = 0;
            v.texcoords[0] = theta/(2*PI);
            v.texcoords[1] = (phi+0.5f*PI)/PI;
            vertexData.push_back(v);
		}
	}
	for (i=0;i<STACKS;i++)
	{
		for (j=0;j<SLICES;j++)
		{
            triangleIndices.push_back(i*(SLICES+1)+j);
            triangleIndices.push_back(i*(SLICES+1)+j+1);
            triangleIndices.push_back((i+1)*(SLICES+1)+j+1);

            triangleIndices.push_back(i*(SLICES+1)+j);
            triangleIndices.push_back((i+1)*(SLICES+1)+j+1);
            triangleIndices.push_back((i+1)*(SLICES+1)+j);

		}
	}
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,buffers[VertexBuffer]);
    glBufferData(GL_ARRAY_BUFFER,sizeof(VertexAttribs)*vertexData.size(),&vertexData[0],GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[IndexBuffer]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint)*triangleIndices.size(), &triangleIndices[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER,buffers[VertexBuffer]);
    glVertexAttribPointer(vPosition,4,GL_FLOAT,GL_FALSE,sizeof(VertexAttribs),BUFFER_OFFSET(0));
    glVertexAttribPointer(vNormal,4,GL_FLOAT,GL_FALSE,sizeof(VertexAttribs),BUFFER_OFFSET(4*sizeof(float)));
    glVertexAttribPointer(vTexCoord,2,GL_FLOAT,GL_FALSE,sizeof(VertexAttribs),BUFFER_OFFSET(8*sizeof(float)));
    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vNormal);
    glEnableVertexAttribArray(vTexCoord);
    glBindVertexArray(0);
}

Sphere::~Sphere(void)
{
}
