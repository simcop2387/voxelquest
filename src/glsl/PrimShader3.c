for (i = 0; i < n; i++) {
    
    
    minPlanes = floor(p);
    maxPlanes = minPlanes + sign(rd);
    
    
    centerPoint = roundVal(p);
    
    if (all(equal(centerPoint,lastPoint)) ) {
        
    }
    else {
        seed = curTime+dot(centerPoint,oneVec);
        globBoxPower = getBoxPower(seed);
        //powerMin = globBoxPower;//max(globBoxPower,vec2(0.0));
        globBoxDim = getBoxDim(seed);
        radMod.y = globBoxDim.w;
        hitBox = aabbIntersect(p1,rd,centerPoint-globBoxDim.xyz-globBoxDim.w, centerPoint+globBoxDim.xyz+globBoxDim.w);

        tempVal = getTer3D(centerPoint);


        if (
            (hitBox.z > 0.0)
            && (tempVal > 0.5)
        ) {
            
            // hitBox1 = aabbIntersect(p1,rd,centerPoint-globBoxDim.xyz-radMod.xxy, centerPoint+globBoxDim.xyz+radMod.xxy);
            // hitBox2 = aabbIntersect(p1,rd,centerPoint-globBoxDim.xyz-radMod.xyx, centerPoint+globBoxDim.xyz+radMod.xyx);
            // hitBox3 = aabbIntersect(p1,rd,centerPoint-globBoxDim.xyz-radMod.yxx, centerPoint+globBoxDim.xyz+radMod.yxx);
            
            // rdTest = rd*hitBox.y - rd*hitBox.x;
            
            // if (
            //     ((hitBox1.z+hitBox2.z+hitBox3.z) > 0.0)
                
            //     || (
            //         (length(rdTest.xy) > globBoxDim.w) ||
            //         (length(rdTest.yz) > globBoxDim.w) ||
            //         (length(rdTest.xz) > globBoxDim.w)
            //     )
                
            //     //|| ((hitBox.y-hitBox.x) > (globBoxDim.w))
            // ) {}
                
                
                
                
            // hitPoint1 = ro + rd*hitBox.x;
            // hitPoint2 = ro + rd*hitBox.y;
            
            // nearestP1 = hitPoint1-centerPoint;
            // nearestP1 = sign(nearestP1)*globBoxDim.xyz + centerPoint;
            // nearestRayPoint = pointSegDistance(vec3(nearestP1.xy,0.0),vec3(hitPoint1.xy,0.0),vec3(hitPoint2.xy,0.0)).xyz;
            // dist1 = powDis( nearestRayPoint.xy - nearestP1.xy, globBoxDim.xy, powerMin );
            // dist1 *= float( all(greaterThan(abs(hitPoint1.xy-centerPoint.xy), globBoxDim.xy)) );
            
            
            
            
            // nearestP2 = hitPoint2-centerPoint;
            // nearestP2 = sign(nearestP2)*globBoxDim.xyz + centerPoint;
            // nearestRayPoint = pointSegDistance(vec3(nearestP2.xy,0.0),vec3(hitPoint1.xy,0.0),vec3(hitPoint2.xy,0.0)).xyz;
            // dist2 = powDis( nearestRayPoint.xy - nearestP2.xy, globBoxDim.xy, powerMin );
            // dist2 *= float( all(greaterThan(abs(hitPoint2.xy-centerPoint.xy), globBoxDim.xy)) );
            
            
            
            // dist2 = pointSegDistance(vec3(nearestP1.yz,0.0),vec3(hitPoint1.yz,0.0),vec3(hitPoint2.yz,0.0)).w;
            // dist2 *= float( all(greaterThan(abs(hitPoint1.yz-centerPoint.yz), globBoxDim.yz)) );
            
            // dist3 = pointSegDistance(vec3(nearestP1.xz,0.0),vec3(hitPoint1.xz,0.0),vec3(hitPoint2.xz,0.0)).w;
            // dist3 *= float( all(greaterThan(abs(hitPoint1.xz-centerPoint.xz), globBoxDim.xz)) );
            
            
            
            
            
            
            
            // if (testOn&&(hitBox.x > 0.1)) {
            //     globTotSteps = float(dist1 <= globBoxDim.w);
            //     break;
            // }
            
            camDis.x = distance(cameraPos,p);
            camDis.y = camDis.x;
            
            // 1.0 = close, 0.0 = far away
            curLOD = mix(
                maxLOD,
                minLOD,
                clamp(camDis/MAX_DETAIL_DIS,minLOD,maxLOD)
            )*accuracy;
            
            btSteps = mix(MIN_STEPS,MAX_STEPS,curLOD);
            
            globCenterPoint = centerPoint;
            
            seBoxDis = 0.0;
            tt = hitBox.x;
            for (j = 0.0; j < btSteps.x; j += 1.0) {
                curPoint = (ro+rd*tt);
                seBoxDis = udRoundBox( centerPoint, curPoint-centerPoint, globBoxDim, globBoxPower );
                tt += seBoxDis;                    
            }
            
            globTotBoxSteps += j;
            
            minMaxT.x = tt;
            minMaxT.y = hitBox.y;
            
            //minMaxT.xy = hitBox.xy;
            
            //pointRectDist(centerPoint, curPoint-centerPoint);
            
            
            
            // hitPoint1 = ro + hitBox.x*rd;
            // hitPoint2 = ro + hitBox.y*rd;
            
            
            
            
            // hitPoint1 = max((abs(hitPoint1 - centerPoint)),0.0);
            // hitPoint2 = max((abs(hitPoint2 - centerPoint)),0.0);
            
            //(length(max(abs(vectorFromCenter)-boxDim.xyz,0.0))-boxDim.w);
            
            // hitPoint1.xyz = ((max(abs(hitPoint1.xyz-centerPoint.xyz)-globBoxDim.xyz,0.0)))/globBoxDim.w;
            // hitPoint2.xyz = ((max(abs(hitPoint2.xyz-centerPoint.xyz)-globBoxDim.xyz,0.0)))/globBoxDim.w;
            
            // hitPoint1.x = length(hitPoint1.xy);
            // hitPoint1.y = length(hitPoint1.xz);
            
            // hitPoint2.x = length(hitPoint2.xy);
            // hitPoint2.y = length(hitPoint2.xz);
            
            // seBoxDis = float(
            //     //(hitPoint1.x >= 0.9999) &&
            //     //(hitPoint2.x >= 0.9999)
            //     // (hitPoint1.y >= 0.9999) &&
            //     // (hitPoint2.y >= 0.9999)
                
            //     (hitBox.y-hitBox.x) > (globBoxDim.w/sqrt(2.0))
                
            // );// *float(hitBox.y-hitBox.x < globBoxDim.w/sqrt(2.0) ); 
            
            // (
            //     distance(hitPoint1,min(hitPoint1,globBoxDim.xyz) ) +
            //     distance(hitPoint2,min(hitPoint2,globBoxDim.xyz) )
            // );//max(length(hitPoint1),length(hitPoint2));
            //length(hitPoint1.xyz);
            //float(hitBox.y-hitBox.x < globBoxDim.w);
            
            
            
            if (
                //(dist1 < globBoxDim.w)
                //min(dist1,dist2) < globBoxDim.w
                
                (seBoxDis <= MAX_BOX_DIS) && 
                (tt > 0.01)
                
                //&& ((hitBox.y-hitBox.x) > (globBoxDim.w)*0.6 )
                
            ) {
              
              tval = rayCast(
                  p1,
                  rd,
                  minMaxT.xy,
                  
                  btSteps.y
                  
              );
                 
              if (
                  (tval < hitBox.y)
                  && (tval > 0.01)
              ) {
                  globTotLS += float(i);
                  return tval;
              }
              
              if (globCurSteps >= MAX_DET_STEPS) {
                break;
              }
               
            }
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
            
        }
    }
    
    p += s*0.5;
    
    lastPoint = centerPoint;
    
    if (distance(p,ro) > maxDis) {
        break;
    }
}
