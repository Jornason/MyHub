#include "Cone.h"
Cone::Cone(string name)
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
            v.position[0] = (i/STACKS * cos(theta))*1.0f;
            v.position[2] = (i/STACKS * sin(theta))*1.0f;
            v.position[1] = (1-(i/STACKS))*1.0f;
            v.position[3] = 1.0f;
            v.normal[0] = (STACKS * cos(phi)) / (pow((((STACKS * cos(phi))* STACKS * cos(phi)) + ((STACKS * sin(phi)) * (STACKS * sin(phi))) + 1)*1.0f, 0.5f));
            v.normal[1] = (STACKS * sin(phi)) / (pow((((STACKS * cos(phi))* STACKS * cos(phi)) + ((STACKS * sin(phi)) * (STACKS * sin(phi))) + 1)*1.0f, 0.5f));
            v.normal[2] = 1 / (pow((((STACKS * cos(phi))* STACKS * cos(phi)) + ((STACKS * sin(phi)) * (STACKS * sin(phi))) + 1)*1.0f, 0.5f));
            v.normal[3] = 0;
            v.texcoords[0] = theta/(2*PI);
            v.texcoords[1] = (phi+0.5f*PI)/PI;
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
    for (j=0;j<SLICES;j++)
    {
        triangleIndices.push_back(2*(STACKS+1)*(SLICES+1) + j);
        triangleIndices.push_back(2*(STACKS+1)*(SLICES+1) + j+1);
        triangleIndices.push_back(2*(STACKS+1)*(SLICES+1) + (SLICES+1) + j+1);
        triangleIndices.push_back(2*(STACKS+1)*(SLICES+1) + j);
        triangleIndices.push_back(2*(STACKS+1)*(SLICES+1) + (SLICES+1) + j+1);
        triangleIndices.push_back(2*(STACKS+1)*(SLICES+1) + (SLICES+1) + j);

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

Cone::~Cone(void)
{
}
