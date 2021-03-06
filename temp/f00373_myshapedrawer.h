// f00373_myshapedrawer.h
//

#include "f00373_myshapedrawer.e"
#define LZZ_INLINE inline
MyShapeDrawer::MyShapeDrawer (Singleton * _singleton)
                {
			singleton = _singleton;
			
			//m_texturehandle			=	0;
			//m_textureenabled		=	false;
			//m_textureinitialized	=	false;
		}
void MyShapeDrawer::init ()
                            {
			
		}
bool MyShapeDrawer::enableTexture (bool enable)
                                                {
			return false;//bool p=m_textureenabled;m_textureenabled=enable;return(p);
		}
bool MyShapeDrawer::hasTextureEnabled () const
                                               {
			return false;//m_textureenabled;
		}
void MyShapeDrawer::drawCoordSystem ()
                                        {
			// glBegin(GL_LINES);
			// //glColor3f(1, 0, 0);
			// glVertex3d(0, 0, 0);
			// glVertex3d(1, 0, 0);
			// //glColor3f(0, 1, 0);
			// glVertex3d(0, 0, 0);
			// glVertex3d(0, 1, 0);
			// //glColor3f(0, 0, 1);
			// glVertex3d(0, 0, 0);
			// glVertex3d(0, 0, 1);
			// glEnd();

		}
void MyShapeDrawer::drawSphere (btScalar radius, int lats, int longs)
                {
			
			int i, j;
			for(i = 0; i <= lats; i++) {
				btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar) (i - 1) / lats);
				btScalar z0  = radius*sin(lat0);
				btScalar zr0 =  radius*cos(lat0);

				btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar) i / lats);
				btScalar z1 = radius*sin(lat1);
				btScalar zr1 = radius*cos(lat1);

				// TODO: reimplement with tris

				glBegin(GL_QUAD_STRIP);
				for(j = 0; j <= longs; j++) {
					btScalar lng = 2 * SIMD_PI * (btScalar) (j - 1) / longs;
					btScalar x = cos(lng);
					btScalar y = sin(lng);
					glNormal3f(-(x * zr1), -(y * zr1), -z1);
					glVertex3f(x * zr1, y * zr1, z1);
					glNormal3f(-(x * zr0), -(y * zr0), -z0);
					glVertex3f(x * zr0, y * zr0, z0);
				}
				glEnd();
			}
		}
ShapeCache * MyShapeDrawer::cache (btConvexShape * shape)
                {
			ShapeCache*		sc=(ShapeCache*)shape->getUserPointer();
			if(!sc)
			{
				sc=new(btAlignedAlloc(sizeof(ShapeCache),16)) ShapeCache(shape);
				sc->m_shapehull.buildHull(shape->getMargin());
				m_shapecaches.push_back(sc);
				shape->setUserPointer(sc);
				/* Build edges	*/ 
				const int			ni=sc->m_shapehull.numIndices();
				const int			nv=sc->m_shapehull.numVertices();
				const unsigned int*	pi=sc->m_shapehull.getIndexPointer();
				const btVector3*	pv=sc->m_shapehull.getVertexPointer();
				btAlignedObjectArray<ShapeCache::Edge*>	edges;
				sc->m_edges.reserve(ni);
				edges.resize(nv*nv,0);
				for(int i=0;i<ni;i+=3)
				{
					const unsigned int* ti=pi+i;
					const btVector3		nrm=btCross(pv[ti[1]]-pv[ti[0]],pv[ti[2]]-pv[ti[0]]).normalized();
					for(int j=2,k=0;k<3;j=k++)
					{
						const unsigned int	a=ti[j];
						const unsigned int	b=ti[k];
						ShapeCache::Edge*&	e=edges[btMin(a,b)*nv+btMax(a,b)];
						if(!e)
						{
							sc->m_edges.push_back(ShapeCache::Edge());
							e=&sc->m_edges[sc->m_edges.size()-1];
							e->n[0]=nrm;e->n[1]=-nrm;
							e->v[0]=a;e->v[1]=b;
						}
						else
						{
							e->n[1]=nrm;
						}
					}
				}
			}
			return(sc);
		}
void MyShapeDrawer::renderSquareA (float x, float y, float z)
                {
			// glBegin(GL_LINE_LOOP);
			// glVertex3f(x, y, z);
			// glVertex3f(x + 10.f, y, z);
			// glVertex3f(x + 10.f, y + 10.f, z);
			// glVertex3f(x, y + 10.f, z);
			// glEnd();
		}
void MyShapeDrawer::setId (int bodyUID, int limbUID)
                  {
			singleton->setShaderFloat("bodyUID", bodyUID);
			singleton->setShaderFloat("limbUID", limbUID);
		}
void MyShapeDrawer::updateMat2 ()
                                  {
			
			glGetFloatv(GL_MODELVIEW_MATRIX, singleton->viewMatrix.get());
			singleton->setShaderMatrix4x4("objmat",singleton->viewMatrix.get(),1);
		}
void MyShapeDrawer::updateMat ()
                                 {
			int i;
			
			singleton->curObjMatrix.identity();
			
			for (i = 0; i < singleton->objMatrixStack.size(); i++) {
				singleton->curObjMatrix *= singleton->objMatrixStack[i];
			}
			//glGetFloatv(GL_MODELVIEW_MATRIX, singleton->viewMatrix.get());
			singleton->setShaderMatrix4x4("objmat",singleton->curObjMatrix.get(),1);
			
			singleton->curObjMatrix3.set4(singleton->curObjMatrix.get());
			singleton->curObjMatrix3.invert();
			singleton->curObjMatrix3.transpose();
			
			singleton->setShaderMatrix3x3("normalRot",singleton->curObjMatrix3.get(),1);
			
			
			
			//btTransform tr;
			// tr.setFromOpenGLMatrix(singleton->curObjMatrix.get());
			// btQuaternion orn = tr.getRotation();
			// singleton->setShaderVec4("objQuat",orn.getX(),orn.getY(),orn.getZ(),orn.getW());
			
		}
void MyShapeDrawer::pushNewMat (btScalar * m)
                                             {
			singleton->objMatrixStack.push_back(Matrix4(
				m[0],
				m[1],
				m[2],	
				m[3],
				
				m[4],
				m[5],
				m[6],
				m[7],
				
				m[8],
				m[9],
				m[10],
				m[11],
				
				m[12],
				m[13],
				m[14],
				m[15]
			));
			updateMat();
		}
void MyShapeDrawer::popMat ()
                              {
			singleton->objMatrixStack.pop_back();
			updateMat();
		}
void MyShapeDrawer::drawOrient (int uid)
                                         {
			
			
			if (uid == singleton->gem->getCurActorUID()) {
				
			}
			else {
				return;
			}
			
			//cout << "yay\n";
			
			int oldUID = uid;
			
			//setId(0);
			
			singleton->setShaderVec3("matVal", 255, 0, 0);
			glBegin(GL_LINES);
			//glNormal3f(0, 0, 1);
			glVertex3d(0, 0, 0);
			glVertex3d(4, 0, 0);
			glEnd();
			
			singleton->setShaderVec3("matVal", 0, 255, 0);
			glBegin(GL_LINES);
			//glNormal3f(0, 0, 1);
			glVertex3d(0, 0, 0);
			glVertex3d(0, 4, 0);
			glEnd();
			
			singleton->setShaderVec3("matVal",0, 0, 255);
			glBegin(GL_LINES);
			//glNormal3f(0, 0, 1);
			glVertex3d(0, 0, 0);
			glVertex3d(0, 0, 4);
			glEnd();
			
			//setId(oldUID);
			singleton->setShaderVec3("matVal", 1, 1, 1);
			
		}
void MyShapeDrawer::drawOpenGL (btScalar * m, btCollisionShape const * shape, btVector3 const & color, int debugMode, btVector3 const & worldBoundsMin, btVector3 const & worldBoundsMax, int uid)
                  {
			
			if (shape->getShapeType() == CUSTOM_CONVEX_SHAPE_TYPE)
			{
				btVector3 org(m[12], m[13], m[14]);
				btVector3 dx(m[0], m[1], m[2]);
				btVector3 dy(m[4], m[5], m[6]);
		//		btVector3 dz(m[8], m[9], m[10]);
				const btBoxShape* boxShape = static_cast<const btBoxShape*>(shape);
				btVector3 halfExtent = boxShape->getHalfExtentsWithMargin();
				dx *= halfExtent[0];
				dy *= halfExtent[1];
		//		dz *= halfExtent[2];
				//glColor3f(1,1,1);
				//glDisable(GL_LIGHTING);

				// glBegin(GL_LINE_LOOP);
				// glDrawVector(org - dx - dy);
				// glDrawVector(org - dx + dy);
				// glDrawVector(org + dx + dy);
				// glDrawVector(org + dx - dy);
				// glEnd();
				return;
			} 
			else if((shape->getShapeType() == BOX_SHAPE_PROXYTYPE) && (debugMode & btIDebugDraw::DBG_FastWireframe))
			{
				btVector3 org(m[12], m[13], m[14]);
				btVector3 dx(m[0], m[1], m[2]);
				btVector3 dy(m[4], m[5], m[6]);
				btVector3 dz(m[8], m[9], m[10]);
				const btBoxShape* boxShape = static_cast<const btBoxShape*>(shape);
				btVector3 halfExtent = boxShape->getHalfExtentsWithMargin();
				dx *= halfExtent[0];
				dy *= halfExtent[1];
				dz *= halfExtent[2];
				// glBegin(GL_LINE_LOOP);
				// glDrawVector(org - dx - dy - dz);
				// glDrawVector(org + dx - dy - dz);
				// glDrawVector(org + dx + dy - dz);
				// glDrawVector(org - dx + dy - dz);
				// glDrawVector(org - dx + dy + dz);
				// glDrawVector(org + dx + dy + dz);
				// glDrawVector(org + dx - dy + dz);
				// glDrawVector(org - dx - dy + dz);
				// glEnd();
				// glBegin(GL_LINES);
				// glDrawVector(org + dx - dy - dz);
				// glDrawVector(org + dx - dy + dz);
				// glDrawVector(org + dx + dy - dz);
				// glDrawVector(org + dx + dy + dz);
				// glDrawVector(org - dx - dy - dz);
				// glDrawVector(org - dx + dy - dz);
				// glDrawVector(org - dx - dy + dz);
				// glDrawVector(org - dx + dy + dz);
				// glEnd();
				return;
			}

			//cout << "a\n";
			//glPushMatrix(); 
			//btglMultMatrix(m);
			//updateMat2();
			pushNewMat(m);
			//singleton->gem->gameObjects[uid].rotMat = singleton->curObjMatrix3;


			if (shape->getShapeType() == UNIFORM_SCALING_SHAPE_PROXYTYPE)
			{
				const btUniformScalingShape* scalingShape = static_cast<const btUniformScalingShape*>(shape);
				const btConvexShape* convexShape = scalingShape->getChildShape();
				float	scalingFactor = (float)scalingShape->getUniformScalingFactor();
				{
					btScalar tmpScaling[4][4]={{scalingFactor,0,0,0},
					{0,scalingFactor,0,0},
					{0,0,scalingFactor,0},
					{0,0,0,1}};

					drawOpenGL( (btScalar*)tmpScaling,convexShape,color,debugMode,worldBoundsMin,worldBoundsMax, uid);
				}
				//cout << "b\n";
				//glPopMatrix();
				//updateMat2();
				popMat();
				return;
			}

			if (shape->getShapeType() == COMPOUND_SHAPE_PROXYTYPE)
			{
				const btCompoundShape* compoundShape = static_cast<const btCompoundShape*>(shape);
				for (int i=compoundShape->getNumChildShapes()-1;i>=0;i--)
				{
					btTransform childTrans = compoundShape->getChildTransform(i);
					const btCollisionShape* colShape = compoundShape->getChildShape(i);
					ATTRIBUTE_ALIGNED16(btScalar) childMat[16];
					childTrans.getOpenGLMatrix(childMat);
					drawOpenGL(childMat,colShape,color,debugMode,worldBoundsMin,worldBoundsMax, uid);
				}

			} else
			{
				// if(m_textureenabled&&(!m_textureinitialized))
				// {
				// 	GLubyte*	image=new GLubyte[256*256*4];
				// 	for(int y=0;y<256;++y)
				// 	{
				// 		const int	t=y>>4;
				// 		GLubyte*	pi=image+y*256*3;
				// 		for(int x=0;x<256;++x)
				// 		{
				// 			const int		s=x>>4;
				// 			const GLubyte	b=180;					
				// 			GLubyte			c=b+((s+(t&1))&1)*(255-b);
				// 			pi[0]=pi[1]=pi[2]=pi[3]=c;pi+=3;
				// 		}
				// 	}

				// 	glGenTextures(1,(GLuint*)&m_texturehandle);
				// 	glBindTexture(GL_TEXTURE_2D,m_texturehandle);


		  //             glGenTextures(1,(GLuint*)&m_texturehandle);
		  //              glBindTexture(GL_TEXTURE_2D,m_texturehandle);
				// glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		  //               glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		  //               glTexImage2D(GL_TEXTURE_2D, 0, 3, 256 , 256 , 0, GL_RGB, GL_UNSIGNED_BYTE, image);
				// //glGenerateMipmap(GL_TEXTURE_2D);
				//   delete[] image;

					
				// }

				// glMatrixMode(GL_TEXTURE);
				// glLoadIdentity();
				// glScalef(0.025f,0.025f,0.025f);
				// glMatrixMode(GL_MODELVIEW);

				// static const GLfloat	planex[]={1,0,0,0};
				// //	static const GLfloat	planey[]={0,1,0,0};
				// 	static const GLfloat	planez[]={0,0,1,0};
				// 	glTexGenfv(GL_S,GL_OBJECT_PLANE,planex);
				// 	glTexGenfv(GL_T,GL_OBJECT_PLANE,planez);
				// 	glTexGeni(GL_S,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
				// 	glTexGeni(GL_T,GL_TEXTURE_GEN_MODE,GL_OBJECT_LINEAR);
					//glEnable(GL_TEXTURE_GEN_S);
					//glEnable(GL_TEXTURE_GEN_T);
					//glEnable(GL_TEXTURE_GEN_R);
				//	m_textureinitialized=true;

				
					

				//drawCoordSystem();

				//glEnable(GL_COLOR_MATERIAL);
				// if(m_textureenabled) 
				// {
				// 	//glEnable(GL_TEXTURE_2D);
				// 	glBindTexture(GL_TEXTURE_2D,m_texturehandle);
				// } else
				// {
				// 	//glDisable(GL_TEXTURE_2D);
				// }


				//glColor3f(color.x(),color.y(), color.z());		

				//bool useWireframeFallback = true;

				if (!(debugMode & btIDebugDraw::DBG_DrawWireframe))
				{
					///you can comment out any of the specific cases, and use the default

					///the benefit of 'default' is that it approximates the actual collision shape including collision margin
					//int shapetype=m_textureenabled?MAX_BROADPHASE_COLLISION_TYPES:shape->getShapeType();
					int shapetype=shape->getShapeType();
					switch (shapetype)
					{

						case SPHERE_SHAPE_PROXYTYPE:
						{
							const btSphereShape* sphereShape = static_cast<const btSphereShape*>(shape);
							float radius = sphereShape->getMargin();//radius doesn't include the margin, so draw with margin
							drawSphere(radius,10,10);
							//useWireframeFallback = false;
							break;
						}

						case BOX_SHAPE_PROXYTYPE:
						{
							//ffffffffff
							
							const btBoxShape* boxShape = static_cast<const btBoxShape*>(shape);
							btVector3 halfExtent = boxShape->getHalfExtentsWithMargin();
							
							static int indices[36] = {
								0,1,2,
								3,2,1,
								4,0,6,
								6,0,2,
								5,1,4,
								4,1,0,
								7,3,1,
								7,1,5,
								5,4,7,
								7,4,6,
								7,2,3,
								7,6,2};

							 btVector3 vertices[8]={	
								btVector3(halfExtent[0],halfExtent[1],halfExtent[2]),
								btVector3(-halfExtent[0],halfExtent[1],halfExtent[2]),
								btVector3(halfExtent[0],-halfExtent[1],halfExtent[2]),	
								btVector3(-halfExtent[0],-halfExtent[1],halfExtent[2]),	
								btVector3(halfExtent[0],halfExtent[1],-halfExtent[2]),
								btVector3(-halfExtent[0],halfExtent[1],-halfExtent[2]),	
								btVector3(halfExtent[0],-halfExtent[1],-halfExtent[2]),	
								btVector3(-halfExtent[0],-halfExtent[1],-halfExtent[2])};
		#if 1
							
							if (singleton->drawOrient) {
								drawOrient(uid);
							}
							else {
								glBegin (GL_TRIANGLES);
								int si=36;
								for (int i=0;i<si;i+=3)
								{
									const btVector3& v1 = vertices[indices[i]];;
									const btVector3& v2 = vertices[indices[i+1]];
									const btVector3& v3 = vertices[indices[i+2]];
									btVector3 normal = (v3-v1).cross(v2-v1);
									normal.normalize ();
									glNormal3f(normal.getX(),normal.getY(),normal.getZ());
									glVertex3f (v1.x(), v1.y(), v1.z());
									glVertex3f (v2.x(), v2.y(), v2.z());
									glVertex3f (v3.x(), v3.y(), v3.z());
									
								}
								glEnd();
							}
							
							
							
		#endif

							//useWireframeFallback = false;
							break;
						}



		#if 0
					
					case CONE_SHAPE_PROXYTYPE:
						{
							const btConeShape* coneShape = static_cast<const btConeShape*>(shape);
							int upIndex = coneShape->getConeUpIndex();
							float radius = coneShape->getRadius();//+coneShape->getMargin();
							float height = coneShape->getHeight();//+coneShape->getMargin();
							switch (upIndex)
							{
							case 0:
								glRotatef(90.0, 0.0, 1.0, 0.0);
								break;
							case 1:
								glRotatef(-90.0, 1.0, 0.0, 0.0);
								break;
							case 2:
								break;
							default:
								{
								}
							};

							glTranslatef(0.0, 0.0, -0.5*height);
							glutSolidCone(radius,height,10,10);
							//useWireframeFallback = false;
							break;

						}
		#endif

					case STATIC_PLANE_PROXYTYPE:
						{
							const btStaticPlaneShape* staticPlaneShape = static_cast<const btStaticPlaneShape*>(shape);
							btScalar planeConst = staticPlaneShape->getPlaneConstant();
							const btVector3& planeNormal = staticPlaneShape->getPlaneNormal();
							btVector3 planeOrigin = planeNormal * planeConst;
							btVector3 vec0,vec1;
							btPlaneSpace1(planeNormal,vec0,vec1);
							btScalar vecLen = 100.f;
							btVector3 pt0 = planeOrigin + vec0*vecLen;
							btVector3 pt1 = planeOrigin - vec0*vecLen;
							btVector3 pt2 = planeOrigin + vec1*vecLen;
							btVector3 pt3 = planeOrigin - vec1*vecLen;
							// glBegin(GL_LINES);
							// glVertex3f(pt0.getX(),pt0.getY(),pt0.getZ());
							// glVertex3f(pt1.getX(),pt1.getY(),pt1.getZ());
							// glVertex3f(pt2.getX(),pt2.getY(),pt2.getZ());
							// glVertex3f(pt3.getX(),pt3.getY(),pt3.getZ());
							// glEnd();


							break;

						}




					case MULTI_SPHERE_SHAPE_PROXYTYPE:
					{
						const btMultiSphereShape* multiSphereShape = static_cast<const btMultiSphereShape*>(shape);

						btTransform childTransform;
						childTransform.setIdentity();

						
						for (int i = multiSphereShape->getSphereCount()-1; i>=0;i--)
						{
							btSphereShape sc(multiSphereShape->getSphereRadius(i));
							childTransform.setOrigin(multiSphereShape->getSpherePosition(i));
							ATTRIBUTE_ALIGNED16(btScalar) childMat[16];
							childTransform.getOpenGLMatrix(childMat);
							drawOpenGL(childMat,&sc,color,debugMode,worldBoundsMin,worldBoundsMax, uid);
						}

						break;
					}

					default:
						{
							if (shape->isConvex())
							{
								const btConvexPolyhedron* poly = shape->isPolyhedral() ? ((btPolyhedralConvexShape*) shape)->getConvexPolyhedron() : 0;
								if (poly)
								{
									int i;
									
									if (singleton->drawOrient) {
										drawOrient(uid);
									}
									else {
										
										glBegin (GL_TRIANGLES);
										for (i=0;i<poly->m_faces.size();i++)
										{
											btVector3 centroid(0,0,0);
											int numVerts = poly->m_faces[i].m_indices.size();
											if (numVerts>2)
											{
												btVector3 v1 = poly->m_vertices[poly->m_faces[i].m_indices[0]];
												for (int v=0;v<poly->m_faces[i].m_indices.size()-2;v++)
												{
													
													btVector3 v2 = poly->m_vertices[poly->m_faces[i].m_indices[v+1]];
													btVector3 v3 = poly->m_vertices[poly->m_faces[i].m_indices[v+2]];
													btVector3 normal = (v3-v1).cross(v2-v1);
													normal.normalize ();
													glNormal3f(normal.getX(),normal.getY(),normal.getZ());
													glVertex3f (v1.x(), v1.y(), v1.z());
													glVertex3f (v2.x(), v2.y(), v2.z());
													glVertex3f (v3.x(), v3.y(), v3.z());
												}
											}
										}
										glEnd ();
									}
									
									
								} else
								{
									ShapeCache*	sc=cache((btConvexShape*)shape);
									//glutSolidCube(1.0);
									btShapeHull* hull = &sc->m_shapehull/*(btShapeHull*)shape->getUserPointer()*/;

									if (hull->numTriangles () > 0)
									{
										int index = 0;
										const unsigned int* idx = hull->getIndexPointer();
										const btVector3* vtx = hull->getVertexPointer();

										if (singleton->drawOrient) {
											drawOrient(uid);
										}
										else {
											glBegin (GL_TRIANGLES);

											for (int i = 0; i < hull->numTriangles (); i++)
											{
												int i1 = index++;
												int i2 = index++;
												int i3 = index++;
												btAssert(i1 < hull->numIndices () &&
													i2 < hull->numIndices () &&
													i3 < hull->numIndices ());

												int index1 = idx[i1];
												int index2 = idx[i2];
												int index3 = idx[i3];
												btAssert(index1 < hull->numVertices () &&
													index2 < hull->numVertices () &&
													index3 < hull->numVertices ());

												btVector3 v1 = vtx[index1];
												btVector3 v2 = vtx[index2];
												btVector3 v3 = vtx[index3];
												btVector3 normal = (v3-v1).cross(v2-v1);
												normal.normalize();
												glNormal3f(normal.getX(),normal.getY(),normal.getZ());
												glVertex3f (v1.x(), v1.y(), v1.z());
												glVertex3f (v2.x(), v2.y(), v2.z());
												glVertex3f (v3.x(), v3.y(), v3.z());

											}
											glEnd ();
										}
										

									}
								}
							}
							else {
								
								
								// btBvhTriangleMeshShape* concaveMesh = (btBvhTriangleMeshShape*) shape;
								// GlDrawcallback drawCallback;
								// drawCallback.m_wireframe = false;
								
								// glBegin(GL_TRIANGLES);
								
								// concaveMesh->processAllTriangles(&drawCallback,worldBoundsMin,worldBoundsMax);
								
								// glEnd();
								
								
							}
						}
					}

				}


				//glNormal3f(0,1,0);
			

				/// for polyhedral shapes
				if (debugMode==btIDebugDraw::DBG_DrawFeaturesText && (shape->isPolyhedral()))
				{
					btPolyhedralConvexShape* polyshape = (btPolyhedralConvexShape*) shape;

					{

						//glColor3f(1.f, 1.f, 1.f);
						int i;
						for (i=0;i<polyshape->getNumVertices();i++)
						{
							btVector3 vtx;
							polyshape->getVertex(i,vtx);
							char buf[12];
							sprintf(buf," %d",i);
							//btDrawString(BMF_GetFont(BMF_kHelvetica10),buf);
						}

						for (i=0;i<polyshape->getNumPlanes();i++)
						{
							btVector3 normal;
							btVector3 vtx;
							polyshape->getPlane(normal,vtx,i);
							//btScalar d = vtx.dot(normal);

							//char buf[12];
							//sprintf(buf," plane %d",i);
							//btDrawString(BMF_GetFont(BMF_kHelvetica10),buf);

						}
					}

				}








			}
			//cout << "c\n";
			//glPopMatrix();
			//updateMat2();
			popMat();

			//glDisable(GL_TEXTURE_2D);
		}
MyShapeDrawer::~ MyShapeDrawer ()
                {
			int i;
			for (i=0;i<m_shapecaches.size();i++)
			{
				m_shapecaches[i]->~ShapeCache();
				btAlignedFree(m_shapecaches[i]);
			}
			m_shapecaches.clear();
			/*if(m_textureinitialized)
			{
			//	glDeleteTextures(1,(const GLuint*) &m_texturehandle);
			}*/
		}
void MyShapeDrawer::drawSceneInternal (btDiscreteDynamicsWorld const * dynamicsWorld, int pass)
                {

			btAssert(dynamicsWorld);

			
			btScalar	m[16];
			btMatrix3x3	rot;rot.setIdentity();
			const int	numObjects=dynamicsWorld->getNumCollisionObjects();
			btVector3 wireColor(1,0,0);
			
			
			
			for(int i=0;i<numObjects;i++)
			{
				const btCollisionObject*	colObj=dynamicsWorld->getCollisionObjectArray()[i];
				const btRigidBody*		body=btRigidBody::upcast(colObj);
				
				setId( body->bodyUID, body->limbUID );//max(,0) );
				
				if(body&&body->getMotionState())
				{
					btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
					myMotionState->m_graphicsWorldTrans.getOpenGLMatrix(m);
					rot=myMotionState->m_graphicsWorldTrans.getBasis();
				}
				else
				{
					colObj->getWorldTransform().getOpenGLMatrix(m);
					rot=colObj->getWorldTransform().getBasis();
				}
				btVector3 wireColor(1.f,1.0f,0.5f); //wants deactivation
				if(i&1) wireColor=btVector3(0.f,0.0f,1.f);
				///color differently for active, sleeping, wantsdeactivation states
				if (colObj->getActivationState() == 1) //active
				{
					if (i & 1)
					{
						wireColor += btVector3 (1.f,0.f,0.f);
					}
					else
					{
						wireColor += btVector3 (.5f,0.f,0.f);
					}
				}
				if(colObj->getActivationState()==2) //ISLAND_SLEEPING
				{
					if(i&1)
					{
						wireColor += btVector3 (0.f,1.f, 0.f);
					}
					else
					{
						wireColor += btVector3 (0.f,0.5f,0.f);
					}
				}

				btVector3 aabbMin(0,0,0),aabbMax(0,0,0);
				//m_dynamicsWorld->getBroadphase()->getBroadphaseAabb(aabbMin,aabbMax);

				aabbMin-=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
				aabbMax+=btVector3(BT_LARGE_FLOAT,BT_LARGE_FLOAT,BT_LARGE_FLOAT);
			//		printf("aabbMin=(%f,%f,%f)\n",aabbMin.getX(),aabbMin.getY(),aabbMin.getZ());
			//		printf("aabbMax=(%f,%f,%f)\n",aabbMax.getX(),aabbMax.getY(),aabbMax.getZ());
			//		m_dynamicsWorld->getDebugDrawer()->drawAabb(aabbMin,aabbMax,btVector3(1,1,1));

				//switch(pass)
				
				//if (!(getDebugMode()& btIDebugDraw::DBG_DrawWireframe))
				int debugMode = 0;//getDebugMode()
				//btVector3 m_sundirection(-1,-1,-1);
				btVector3 m_sundirection(btVector3(1,-2,1)*1000);
						
				// switch(pass)
				// {
				// 	// case	0:	drawOpenGL(m,colObj->getCollisionShape(),wireColor,debugMode,aabbMin,aabbMax);break;
				// 	// case	1:	drawShadow(m,m_sundirection*rot,colObj->getCollisionShape(),aabbMin,aabbMax);break;
				// 	//case	2:	drawOpenGL(m,colObj->getCollisionShape(),wireColor*btScalar(0.3),0,aabbMin,aabbMax);break;
					
					
				// }
				
				
				bool doProc = true;
				
				if (body->bodyUID >= 0) {
					BaseObj* ge = &(singleton->gem->gameObjects[body->bodyUID]);
					
					if (body->limbUID >= 0) {
						doProc = ge->bodies[body->limbUID].isVisible;
					}
				}
				else {
					doProc = true;
				}
				
				
				
				// if (
				// 	(body->bodyUID == singleton->gem->getCurActorUID())&&singleton->orgOn
				// ) {
					
				// }
				// else {
				
				if (doProc) {
					drawOpenGL(m,colObj->getCollisionShape(),wireColor,debugMode,aabbMin,aabbMax, body->bodyUID);
				}
				
					
				//}
				
				
				//drawOpenGL(m,colObj->getCollisionShape(),wireColor*btScalar(0.3),0,aabbMin,aabbMax);
			}

		}
void MyShapeDrawer::drawScene (btDiscreteDynamicsWorld const * dynamicsWorld, bool useShadows)
                {
		/*
			GLfloat light_ambient[] = { btScalar(0.2), btScalar(0.2), btScalar(0.2), btScalar(1.0) };
			GLfloat light_diffuse[] = { btScalar(1.0), btScalar(1.0), btScalar(1.0), btScalar(1.0) };
			GLfloat light_specular[] = { btScalar(1.0), btScalar(1.0), btScalar(1.0), btScalar(1.0 )};
			
			GLfloat light_position0[] = { btScalar(1.0), btScalar(10.0), btScalar(1.0), btScalar(0.0 )};
			GLfloat light_position1[] = { btScalar(-1.0), btScalar(-10.0), btScalar(-1.0), btScalar(0.0) };

			glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
			glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
			glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
			glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

			glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
			glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
			glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
			glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

			glEnable(GL_LIGHTING);
			glEnable(GL_LIGHT0);
			glEnable(GL_LIGHT1);


			glShadeModel(GL_SMOOTH);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			glClearColor(btScalar(0.7),btScalar(0.7),btScalar(0.7),btScalar(0));


			if(useShadows)
				{
					glClear(GL_STENCIL_BUFFER_BIT);
					glEnable(GL_CULL_FACE);
					drawSceneInternal(dynamicsWorld,0);

					glDisable(GL_LIGHTING);
					glDepthMask(GL_FALSE);
					glDepthFunc(GL_LEQUAL);
					glEnable(GL_STENCIL_TEST);
					glColorMask(GL_FALSE,GL_FALSE,GL_FALSE,GL_FALSE);
					glStencilFunc(GL_ALWAYS,1,0xFFFFFFFFL);
					glFrontFace(GL_CCW);
					glStencilOp(GL_KEEP,GL_KEEP,GL_INCR);
					drawSceneInternal(dynamicsWorld,1);
					glFrontFace(GL_CW);
					glStencilOp(GL_KEEP,GL_KEEP,GL_DECR);
					drawSceneInternal(dynamicsWorld,1);
					glFrontFace(GL_CCW);

					glPolygonMode(GL_FRONT,GL_FILL);
					glPolygonMode(GL_BACK,GL_FILL);
					glShadeModel(GL_SMOOTH);
					glEnable(GL_DEPTH_TEST);
					glDepthFunc(GL_LESS);
					glEnable(GL_LIGHTING);
					glDepthMask(GL_TRUE);
					glCullFace(GL_BACK);
					glFrontFace(GL_CCW);
					glEnable(GL_CULL_FACE);
					glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);

					glDepthFunc(GL_LEQUAL);
					glStencilFunc( GL_NOTEQUAL, 0, 0xFFFFFFFFL );
					glStencilOp( GL_KEEP, GL_KEEP, GL_KEEP );
					glDisable(GL_LIGHTING);
					drawSceneInternal(dynamicsWorld,2);
					glEnable(GL_LIGHTING);
					glDepthFunc(GL_LESS);
					glDisable(GL_STENCIL_TEST);
					glDisable(GL_CULL_FACE);
				}
				else
				{
					glDisable(GL_CULL_FACE);
					drawSceneInternal(dynamicsWorld,0);
				}
			*/
				
			drawSceneInternal(dynamicsWorld,0);
		}
#undef LZZ_INLINE
 
