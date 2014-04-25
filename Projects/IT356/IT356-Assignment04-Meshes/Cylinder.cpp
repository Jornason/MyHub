#include "Cylinder.h"

Cylinder::Cylinder(string name,float inner_radius)
    :Object(name)
{
	float theta;
    int STACKS=1;
	int i,j;
    int SLICES = 360;
    float PI = 3.14159f,y;
	float costheta,sintheta;
    VertexAttribs v;
	for (i=0;i<=STACKS;i++)
	{
		y = (float)i/STACKS;
		for (j=0;j<=SLICES;j++)
		{
			theta = 2*j*PI/SLICES;
			costheta = cos(theta);
			sintheta = sin(theta);
            v.position[0] = costheta;
            v.position[1] = y;
            v.position[2] = sintheta;
            v.position[3] = 1;
            v.normal[0] = costheta;
            v.normal[1] = 0;
            v.normal[2] = sintheta;
            v.normal[3] = 0;
            v.texcoords[0] = theta/(2*PI);
            v.texcoords[1] = y;
            vertexData.push_back(v);
		}
	}
    for (i=0;i<=STACKS;i++)
    {
        y = (float)i/STACKS;
        for (j=0;j<=SLICES;j++)
        {
            theta = 2*j*PI/SLICES;
            costheta = cos(theta);
            sintheta = sin(theta);
            v.position[0] = 0;
            v.position[1] = y;
            v.position[2] = 0;
            v.position[3] = 1;
            v.normal[0] = -costheta;
            v.normal[1] = 0;
            v.normal[2] = -sintheta;
            v.normal[3] = 0;
            v.texcoords[0] = theta/(2*PI);
            v.texcoords[1] = y;
            vertexData.push_back(v);
        }
    }
    for (j=0;j<=SLICES;j++)
    {
        theta = 2*j*PI/SLICES;
        costheta = cos(theta);
        sintheta = sin(theta);
        v.position[0] = costheta;
        v.position[1] = 0;
        v.position[2] = sintheta;
        v.position[3] = 1;
        v.normal[0] = 0;
        v.normal[1] = -1;
        v.normal[2] = 0;
        v.normal[3] = 0;
        v.texcoords[0] = theta/(2*PI);
        v.texcoords[1] = 0;
        vertexData.push_back(v);
    }
    for (j=0;j<=SLICES;j++)
    {
        theta = 2*j*PI/SLICES;
        costheta = cos(theta);
        sintheta = sin(theta);
        v.position[0] = inner_radius * costheta;
        v.position[1] = 0;
        v.position[2] = inner_radius * sintheta;
        v.position[3] = 1;
        v.normal[0] = 0;
        v.normal[1] = -1;
        v.normal[2] = 0;
        v.normal[3] = 0;
        v.texcoords[0] = theta/(2*PI);
        v.texcoords[1] = 0;
        vertexData.push_back(v);
    }
    for (j=0;j<=SLICES;j++)
    {
        theta = 2*j*PI/SLICES;
        costheta = cos(theta);
        sintheta = sin(theta);
        v.position[0] = costheta;
        v.position[1] = 1;
        v.position[2] = sintheta;
        v.position[3] = 1;
        v.normal[0] = 0;
        v.normal[1] = 1;
        v.normal[2] = 0;
        v.normal[3] = 0;
        v.texcoords[0] = theta/(2*PI);
        v.texcoords[1] = 1;
        vertexData.push_back(v);
    }
    for (j=0;j<=SLICES;j++)
    {
        theta = 2*j*PI/SLICES;
        costheta = cos(theta);
        sintheta = sin(theta);
        v.position[0] = inner_radius * costheta;
        v.position[1] = 1;
        v.position[2] = inner_radius * sintheta;
        v.position[3] = 1;
        v.normal[0] = 0;
        v.normal[1] = 1;
        v.normal[2] = 0;
        v.normal[3] = 0;
        v.texcoords[0] = theta/(2*PI);
        v.texcoords[1] = 1;
        vertexData.push_back(v);
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
    for (i=0;i<STACKS;i++)
    {
        for (j=0;j<SLICES;j++)
        {
            triangleIndices.push_back((STACKS+1)*(SLICES+1) + i*(SLICES+1)+j);
            triangleIndices.push_back((STACKS+1)*(SLICES+1) + i*(SLICES+1)+j+1);
            triangleIndices.push_back((STACKS+1)*(SLICES+1) + (i+1)*(SLICES+1)+j+1);
            triangleIndices.push_back((STACKS+1)*(SLICES+1) + i*(SLICES+1)+j);
            triangleIndices.push_back((STACKS+1)*(SLICES+1) + (i+1)*(SLICES+1)+j+1);
            triangleIndices.push_back((STACKS+1)*(SLICES+1) + (i+1)*(SLICES+1)+j);
        }
    }
    for (j=0;j<SLICES;j++)
    {
        triangleIndices.push_back(2*(STACKS+1)*(SLICES+1) + j);
        triangleIndices.push_back(2*(STACKS+1)*(SLICES+1) + j+1);
        triangleIndices.push_back(2*(STACKS+1)*(SLICES+1) + (SLICES+1) + j+1);
        triangleIndices.push_back(2*(STACKS+1)*(SLICES+1) + j);
        triangleIndices.push_back(2*(STACKS+1)*(SLICES+1) + (SLICES+1) + j+1);
        triangleIndices.push_back(2*(STACKS+1)*(SLICES+1) + (SLICES+1) + j);
    }
    for (j=0;j<SLICES;j++)
    {
        triangleIndices.push_back(2*(STACKS+1)*(SLICES+1) + 2*(SLICES+1) + j);
        triangleIndices.push_back(2*(STACKS+1)*(SLICES+1) + 2*(SLICES+1) + j+1);
        triangleIndices.push_back(2*(STACKS+1)*(SLICES+1) + 2*(SLICES+1) + (SLICES+1) + j+1);
        triangleIndices.push_back(2*(STACKS+1)*(SLICES+1) + 2*(SLICES+1) + j);
        triangleIndices.push_back(2*(STACKS+1)*(SLICES+1) + 2*(SLICES+1) + (SLICES+1) + j+1);
        triangleIndices.push_back(2*(STACKS+1)*(SLICES+1) + 2*(SLICES+1) + (SLICES+1) + j);
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

Cylinder::~Cylinder()
{
}
