var scale = 1.5;
var newW = 576*scale;
var newH = 768*scale;


var iconArr = new Array(2048);
var ITEMS_PER_ROW = 36;


function doNumber(pCurCount, x1, y1, x2, y2) {
  var i;
  var j;
  
  var curCount = pCurCount;
  
  for (j = y1; j <= y2; j++) {
    for (i = x1; i <= x2; i++) {
      iconArr[i + j * ITEMS_PER_ROW] = curCount;
      curCount++;
    }
  }
  
  
  return curCount;
  
}

j$( document ).ready(function() {
    
    var fieldNameElement = document.getElementById("myId");
    fieldNameElement.innerHTML = "0";
    var fieldNameElement2 = document.getElementById("myId2");
    fieldNameElement2.innerHTML = "0";
    
    var itemCount = 0;
    
    var i;
    
    for (i = 0; i < 2048; i++) {
      iconArr[i] = 0;
    }
    
    itemCount = doNumber(itemCount,0,0,11,20);
    itemCount = doNumber(itemCount,12,0,23,15);
    itemCount = doNumber(itemCount,24,0,35,16);
    itemCount = doNumber(itemCount,12,16,21,20);
    itemCount = doNumber(itemCount,22,17,35,31);
    itemCount = doNumber(itemCount,0,21,15,31);
    itemCount = doNumber(itemCount,16,21,21,22);
    itemCount = doNumber(itemCount,0,32,35,35);
    itemCount = doNumber(itemCount,0,36,15,47);
    itemCount = doNumber(itemCount,16,36,35,47);
    
    
    
    j$("#imgId").css('width',newW);
    j$("#imgId").css('height',newH);
    
    j$("#imgId").mousemove(function(e){
       var wrapper = j$(this).parent();
       var parentOffset = wrapper.offset(); 
       var relX = e.pageX - parentOffset.left + wrapper.scrollLeft();
       var relY = e.pageY - parentOffset.top + wrapper.scrollTop();

       

       var imgWidth = newW;
       var imgHeight = newH;
       
       var iconWidth = (16*scale);
       var iconsPerRow = Math.floor(imgWidth/iconWidth);

       var curInd = (Math.floor(relX/iconWidth) + Math.floor(relY/iconWidth)*iconsPerRow);
       var newText = iconArr[curInd] + //curInd + " >> " + 
       " ("+Math.floor(relX/iconWidth)+","+Math.floor(relY/iconWidth)+")";

       fieldNameElement.innerHTML = newText;
       fieldNameElement2.innerHTML = newText;
        
    });
});


