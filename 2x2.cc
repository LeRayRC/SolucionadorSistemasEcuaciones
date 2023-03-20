#include <stdio.h>

struct TRes2x2
{
  float x,y;
  int code;
};


struct TEq2x2{
  /* 2x2 system*/
  float x,y,it;
};

void Scale(TEq2x2 *eq,float scale){
  eq->x *= scale;
  eq->y *= scale;
  eq->it *= scale;
} 

TRes2x2 Resolve2x2(TEq2x2 *eq1, TEq2x2 *eq2, bool debug=false){
  TRes2x2 res;
  float temp_x1=eq1->x,temp_x2=eq2->x,
        temp_y1=eq1->y,temp_y2=eq2->y;
  if(eq1->x != 0 && eq1->y == 0 && eq2->x == 0 && eq2->y != 0){
    res.x = eq1->it / eq1->x;
    res.y = eq2->it / eq2->y;
    res.code = 0;
    return res;
  }else if(eq1->x == 0 && eq1->y != 0 && eq2->x != 0 && eq2->y == 0){
    res.x = eq2->it / eq2->x;
    res.y = eq1->it / eq1->y;
    res.code = 0;
    return res;
  }
  if(temp_x1>0 || temp_x2<0){
    temp_x1*=-1;
    Scale(eq1,temp_x2);
    Scale(eq2,temp_x1);
  }
  else if(temp_x1 == 0 || temp_x2 == 0){
    temp_y1*=-1;
    Scale(eq1,temp_y2);
    Scale(eq2,temp_y1);
  }else{
    temp_x1*=-1;
    Scale(eq1,temp_x2);
    Scale(eq2,temp_x1);
  }
    if(eq1->x+eq2->x == 0 && eq1->y+eq2->y == 0 && eq1->it+eq2->it != 0){
      // Sistema incompatible
      if(debug){
        printf("Incompatible system, both lines are parallel\n");
      }
      res.code=1;
    }else if(eq1->x+eq2->x == 0 && eq1->y+eq2->y == 0 && eq1->it+eq2->it == 0){
       // Sistema compatible indeterminado
       if(debug){
        printf("Dependant system, both lines are the same\n");
       }
       res.code=2;
    }else if(eq1->x+eq2->x == 0){
      res.y=(eq1->it+eq2->it)/(eq1->y+eq2->y);
      res.x=(eq2->it-(eq2->y*res.y))/eq2->x;
      res.code=0;
    }else{
      //respuesta sin controlar
      if(debug){
        printf("Uncontrolled solution");
        printf("x: %f y: %f it: %f\n",eq1->x,eq1->y,eq1->it);
        printf("x: %f y: %f it: %f\n",eq2->x,eq2->y,eq2->it);
      }
      res.code=-1;
    }
  //if(debug){
  //  printf("x: %f y: %f it: %f\n",eq1->x,eq1->y,eq1->it);
  //  printf("x: %f y: %f it: %f\n",eq2->x,eq2->y,eq2->it);
  //}
  return res;
}