// f00374_myglhelper.h
//

#include "f00374_myglhelper.e"
#define LZZ_INLINE inline
MyGLHelper::MyGLHelper (Singleton * _singleton, CommonGraphicsApp * glApp)
        {
		singleton = _singleton;
		m_data = new MyGLHelperInternalData;
		m_data->m_glApp = glApp;
		m_data->m_debugDraw = 0;
		

		m_data->m_gl2ShapeDrawer = new MyShapeDrawer(singleton);
	}
MyGLHelper::~ MyGLHelper ()
        {
		delete m_data->m_gl2ShapeDrawer;
		delete m_data;
	}
struct CommonRenderInterface * MyGLHelper::getRenderInterface ()
        {
		return m_data->m_glApp->m_renderer;
	}
void MyGLHelper::createRigidBodyGraphicsObject (btRigidBody * body, btVector3 const & color)
        {
		createCollisionObjectGraphicsObject(body,color);
	}
void MyGLHelper::createCollisionObjectGraphicsObject (btCollisionObject * body, btVector3 const & color)
        {
		if (body->getUserIndex()<0)
		{
			btCollisionShape* shape = body->getCollisionShape();
			btTransform startTransform = body->getWorldTransform();
			int graphicsShapeId = shape->getUserIndex();
			if (graphicsShapeId>=0)
			{
			//	btAssert(graphicsShapeId >= 0);
				//the graphics shape is already scaled
				btVector3 localScaling(1,1,1);
				int graphicsInstanceId = m_data->m_glApp->m_renderer->registerGraphicsInstance(graphicsShapeId, startTransform.getOrigin(), startTransform.getRotation(), color, localScaling);
				body->setUserIndex(graphicsInstanceId);
			}
		}
	}
int MyGLHelper::registerGraphicsShape (float const * vertices, int numvertices, int const * indices, int numIndices)
        {
		int shapeId = m_data->m_glApp->m_renderer->registerShape(vertices, numvertices,indices,numIndices);
		return shapeId;
	}
int MyGLHelper::registerGraphicsInstance (int shapeIndex, float const * position, float const * quaternion, float const * color, float const * scaling)
        {
		return m_data->m_glApp->m_renderer->registerGraphicsInstance(shapeIndex,position,quaternion,color,scaling);
	}
void MyGLHelper::createCollisionShapeGraphicsObjectInternal (btCollisionShape * collisionShape, btTransform const & parentTransform, btAlignedObjectArray <GLInstanceVertex> & verticesOut, btAlignedObjectArray <int> & indicesOut)
        {
	//todo: support all collision shape types
		switch (collisionShape->getShapeType())
		{
			case SOFTBODY_SHAPE_PROXYTYPE:
			{
				//skip the soft body collision shape for now
				break;
			}
			case STATIC_PLANE_PROXYTYPE:
			{
				//draw a box, oriented along the plane normal
				const btStaticPlaneShape* staticPlaneShape = static_cast<const btStaticPlaneShape*>(collisionShape);
				btScalar planeConst = staticPlaneShape->getPlaneConstant();
				const btVector3& planeNormal = staticPlaneShape->getPlaneNormal();
				btVector3 planeOrigin = planeNormal * planeConst;
				btVector3 vec0,vec1;
				btPlaneSpace1(planeNormal,vec0,vec1);
				btScalar vecLen = 100.f;
				btVector3 verts[4];

				verts[0] = planeOrigin + vec0*vecLen + vec1*vecLen;
				verts[1] = planeOrigin - vec0*vecLen + vec1*vecLen;
				verts[2] = planeOrigin - vec0*vecLen - vec1*vecLen;
				verts[3] = planeOrigin + vec0*vecLen - vec1*vecLen;
					
				int startIndex = verticesOut.size();
				indicesOut.push_back(startIndex+0);
				indicesOut.push_back(startIndex+1);
				indicesOut.push_back(startIndex+2);
				indicesOut.push_back(startIndex+0);
				indicesOut.push_back(startIndex+2);
				indicesOut.push_back(startIndex+3);

				btVector3 triNormal = parentTransform.getBasis()*planeNormal;
					

				for (int i=0;i<4;i++)
				{
					GLInstanceVertex vtx;
					btVector3 pos =parentTransform*verts[i];
					vtx.xyzw[0] = pos.x();
					vtx.xyzw[1] = pos.y();
					vtx.xyzw[2] = pos.z();
					vtx.xyzw[3] = 0.f;

					vtx.normal[0] =triNormal.x();
					vtx.normal[1] =triNormal.y();
					vtx.normal[2] =triNormal.z();

					vtx.uv[0] = 0.5f;
					vtx.uv[1] = 0.5f;
					verticesOut.push_back(vtx);
				}
				break;
			}
			case TRIANGLE_MESH_SHAPE_PROXYTYPE:
			{
				

				btBvhTriangleMeshShape* trimesh = (btBvhTriangleMeshShape*) collisionShape;
				btVector3 trimeshScaling = trimesh->getLocalScaling();
				btStridingMeshInterface* meshInterface = trimesh->getMeshInterface();
				btAlignedObjectArray<btVector3> vertices;
				btAlignedObjectArray<int> indices;
					
				for (int partId=0;partId<meshInterface->getNumSubParts();partId++)
				{
						
					const unsigned char *vertexbase = 0;
					int numverts = 0;
					PHY_ScalarType type = PHY_INTEGER;
					int stride = 0;
					const unsigned char *indexbase = 0;
					int indexstride = 0;
					int numfaces = 0;
					PHY_ScalarType indicestype = PHY_INTEGER;
					//PHY_ScalarType indexType=0;
						
					btVector3 triangleVerts[3];
					meshInterface->getLockedReadOnlyVertexIndexBase(&vertexbase,numverts,	type,stride,&indexbase,indexstride,numfaces,indicestype,partId);
					btVector3 aabbMin,aabbMax;
						
					for (int triangleIndex = 0 ; triangleIndex < numfaces;triangleIndex++)
					{
						unsigned int* gfxbase = (unsigned int*)(indexbase+triangleIndex*indexstride);
							
						for (int j=2;j>=0;j--)
						{
								
							int graphicsindex = indicestype==PHY_SHORT?((unsigned short*)gfxbase)[j]:gfxbase[j];
							if (type == PHY_FLOAT)
							{
								float* graphicsbase = (float*)(vertexbase+graphicsindex*stride);
								triangleVerts[j] = btVector3(
																graphicsbase[0]*trimeshScaling.getX(),
																graphicsbase[1]*trimeshScaling.getY(),
																graphicsbase[2]*trimeshScaling.getZ());
							}
							else
							{
								double* graphicsbase = (double*)(vertexbase+graphicsindex*stride);
								triangleVerts[j] = btVector3( btScalar(graphicsbase[0]*trimeshScaling.getX()),
																btScalar(graphicsbase[1]*trimeshScaling.getY()),
																btScalar(graphicsbase[2]*trimeshScaling.getZ()));
							}
						}
						indices.push_back(vertices.size());
						vertices.push_back(triangleVerts[0]);
						indices.push_back(vertices.size());
						vertices.push_back(triangleVerts[1]);
						indices.push_back(vertices.size());
						vertices.push_back(triangleVerts[2]);

						btVector3 triNormal = (triangleVerts[1]-triangleVerts[0]).cross(triangleVerts[2]-triangleVerts[0]);
						triNormal.normalize();

						for (int v=0;v<3;v++)
						{
							GLInstanceVertex vtx;
							btVector3 pos =parentTransform*triangleVerts[v];
							vtx.xyzw[0] = pos.x();
							vtx.xyzw[1] = pos.y();
							vtx.xyzw[2] = pos.z();
							vtx.xyzw[3] = 0.f;


							vtx.normal[0] =triNormal.x();
							vtx.normal[1] =triNormal.y();
							vtx.normal[2] =triNormal.z();

							vtx.uv[0] = 0.5f;
							vtx.uv[1] = 0.5f;

							indicesOut.push_back(verticesOut.size());
							verticesOut.push_back(vtx);
						}

						
					}
				}
				
				break;
			}
			default:
			{
				if (collisionShape->isConvex())
				{
					btConvexShape* convex = (btConvexShape*)collisionShape;
					{
						btShapeHull* hull = new btShapeHull(convex);
						hull->buildHull(0.0);

						{
							//int strideInBytes = 9*sizeof(float);
							//int numVertices = hull->numVertices();
							//int numIndices =hull->numIndices();

							for (int t=0;t<hull->numTriangles();t++)
							{

								btVector3 triNormal;

								int index0 = hull->getIndexPointer()[t*3+0];
								int index1 = hull->getIndexPointer()[t*3+1];
								int index2 = hull->getIndexPointer()[t*3+2];
								btVector3 pos0 =parentTransform*hull->getVertexPointer()[index0];
								btVector3 pos1 =parentTransform*hull->getVertexPointer()[index1];
								btVector3 pos2 =parentTransform*hull->getVertexPointer()[index2];
								triNormal = (pos1-pos0).cross(pos2-pos0);
								triNormal.normalize();

								for (int v=0;v<3;v++)
								{
									int index = hull->getIndexPointer()[t*3+v];
									GLInstanceVertex vtx;
									btVector3 pos =parentTransform*hull->getVertexPointer()[index];
									vtx.xyzw[0] = pos.x();
									vtx.xyzw[1] = pos.y();
									vtx.xyzw[2] = pos.z();
									vtx.xyzw[3] = 0.f;

									vtx.normal[0] =triNormal.x();
									vtx.normal[1] =triNormal.y();
									vtx.normal[2] =triNormal.z();

									vtx.uv[0] = 0.5f;
									vtx.uv[1] = 0.5f;

									indicesOut.push_back(verticesOut.size());
									verticesOut.push_back(vtx);
								}
							}
						}
					}
				} else
				{
					if (collisionShape->isCompound())
					{
						btCompoundShape* compound = (btCompoundShape*) collisionShape;
						for (int i=0;i<compound->getNumChildShapes();i++)
						{

							btTransform childWorldTrans = parentTransform * compound->getChildTransform(i);
							createCollisionShapeGraphicsObjectInternal(compound->getChildShape(i),childWorldTrans,verticesOut,indicesOut);
						}
					} else
					{
						btAssert(0);
					}
						
				}
			}
		};
	}
void MyGLHelper::createCollisionShapeGraphicsObject (btCollisionShape * collisionShape)
        {
		//already has a graphics object?
		if (collisionShape->getUserIndex()>=0)
			return;

		btAlignedObjectArray<GLInstanceVertex> vertices;
		btAlignedObjectArray<int> indices;
		btTransform startTrans;startTrans.setIdentity();

		createCollisionShapeGraphicsObjectInternal(collisionShape,startTrans,vertices,indices);

		if (vertices.size() && indices.size())
		{
			int shapeId = m_data->m_glApp->m_renderer->registerShape(&vertices[0].xyzw[0],vertices.size(),&indices[0],indices.size());
			collisionShape->setUserIndex(shapeId);
		}
			
	}
void MyGLHelper::syncPhysicsToGraphics (btDiscreteDynamicsWorld const * rbWorld)
        {
		int numCollisionObjects = rbWorld->getNumCollisionObjects();
		for (int i = 0; i<numCollisionObjects; i++)
		{
						
			
			btCollisionObject* colObj = rbWorld->getCollisionObjectArray()[i];
			btVector3 pos = colObj->getWorldTransform().getOrigin();
			btQuaternion orn = colObj->getWorldTransform().getRotation();
			int index = colObj->getUserIndex();
			if (index >= 0)
			{
				m_data->m_glApp->m_renderer->writeSingleInstanceTransformToCPU(pos, orn, index);
			}
		}
		m_data->m_glApp->m_renderer->writeTransforms();
	}
void MyGLHelper::render (btDiscreteDynamicsWorld const * rbWorld)
        {
		

		m_data->m_glApp->m_renderer->renderScene();
		//backwards compatible OpenGL2 rendering

		if (m_data->m_gl2ShapeDrawer && rbWorld)
		{
			
			
			//m_data->m_gl2ShapeDrawer->enableTexture(true);
			m_data->m_gl2ShapeDrawer->drawScene(rbWorld,true);
		}
	}
void MyGLHelper::createPhysicsDebugDrawer (btDiscreteDynamicsWorld * rbWorld)
        {
		btAssert(rbWorld);
	    m_data->m_debugDraw = new MyDebugDrawer(m_data->m_glApp);
	    rbWorld->setDebugDrawer(m_data->m_debugDraw );


	    m_data->m_debugDraw->setDebugMode(
	        btIDebugDraw::DBG_DrawWireframe
	        +btIDebugDraw::DBG_DrawAabb
	        //btIDebugDraw::DBG_DrawContactPoints
	        );

	}
struct Common2dCanvasInterface * MyGLHelper::get2dCanvasInterface ()
        {
		return m_data->m_glApp->m_2dCanvasInterface;
	}
CommonParameterInterface * MyGLHelper::getParameterInterface ()
        {
		return m_data->m_glApp->m_parameterInterface;
	}
void MyGLHelper::setUpAxis (int axis)
        {
		m_data->m_glApp->setUpAxis(axis);
	}
void MyGLHelper::resetCamera (float camDist, float pitch, float yaw, float camPosX, float camPosY, float camPosZ)
        {
		if (getRenderInterface() && getRenderInterface()->getActiveCamera())
		{
			getRenderInterface()->getActiveCamera()->setCameraDistance(camDist);
			getRenderInterface()->getActiveCamera()->setCameraPitch(pitch);
			getRenderInterface()->getActiveCamera()->setCameraYaw(yaw);
			getRenderInterface()->getActiveCamera()->setCameraTargetPosition(camPosX,camPosY,camPosZ);
		}
	}
void MyGLHelper::autogenerateGraphicsObjects (btDiscreteDynamicsWorld * rbWorld)
        {
		//sort the collision objects based on collision shape, the gfx library requires instances that re-use a shape to be added after eachother

		btAlignedObjectArray<btCollisionObject*> sortedObjects;
		sortedObjects.reserve(rbWorld->getNumCollisionObjects());
		for (int i=0;i<rbWorld->getNumCollisionObjects();i++)
		{
			btCollisionObject* colObj = rbWorld->getCollisionObjectArray()[i];
			sortedObjects.push_back(colObj);
		}
		sortedObjects.quickSort(shapePointerCompareFunc);
		for (int i=0;i<sortedObjects.size();i++)
		{
			btCollisionObject* colObj = sortedObjects[i];
			//btRigidBody* body = btRigidBody::upcast(colObj);
			//does this also work for btMultiBody/btMultiBodyLinkCollider?
			createCollisionShapeGraphicsObject(colObj->getCollisionShape());
			int colorIndex = colObj->getBroadphaseHandle()->getUid() & 3;

			btVector3 color= sColors[colorIndex];
			createCollisionObjectGraphicsObject(colObj,color);
				
		}
	}
void MyGLHelper::drawText3D (char const * txt, float posX, float posY, float posZ, float size)
        {
	    btAssert(m_data->m_glApp);
	    m_data->m_glApp->drawText3D(txt,posX,posY,posZ,size);
	}
struct CommonGraphicsApp * MyGLHelper::getAppInterface ()
        {
		return m_data->m_glApp;
	}
#undef LZZ_INLINE
 
