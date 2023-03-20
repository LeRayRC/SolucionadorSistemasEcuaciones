#include <stdio.h>
#include "2x2.cc"

struct TEq3x3{
  float x,y,z,it;
};

struct TRes3x3{
  TEq3x3 eq;
  int code;
};

TEq3x3 Scale(TEq3x3 *eq,float scale){
  return {eq->x*scale,eq->y*scale,eq->z*scale,eq->it*scale};
} 

TRes3x3 Resolve3x3(TEq3x3 *eq1, TEq3x3 *eq2,TEq3x3 *eq3, bool debug=false){
  // mode 
  // 0 -> resolve y and z
  // 1 -> resolve x and z
  // 2 -> resolve x and y
  char mode;
  TRes3x3 res;
  TVec3 res_2x2;
  TEq2x2 temp_2x2_eq1, temp_2x2_eq2;
  TEq3x3 temp_3x3_eq1, temp_3x3_eq2;
  //first equation with second
  if(eq1->x>0 || (eq2->x<0 || eq3->x<0)){
    temp_3x3_eq1 = Scale(eq1,eq2->x);
    temp_3x3_eq2 = Scale(eq2,eq1->x*-1.0f);
    temp_2x2_eq1 = {temp_3x3_eq1.y + temp_3x3_eq2.y, temp_3x3_eq1.z + temp_3x3_eq2.z, temp_3x3_eq1.it + temp_3x3_eq2.it};
    temp_3x3_eq1 = Scale(eq1,eq3->x);
    temp_3x3_eq2 = Scale(eq3,eq1->x*-1.0f);
    temp_2x2_eq2 = {temp_3x3_eq1.y + temp_3x3_eq2.y, temp_3x3_eq1.z + temp_3x3_eq2.z, temp_3x3_eq1.it + temp_3x3_eq2.it};
    mode = 0;
  }
  else if(eq1->x == 0 || (eq2->x == 0 || eq3->x == 0 )){
    if(eq1->y>0 || (eq2->y<0 || eq3->y<0)){
      temp_3x3_eq1 = Scale(eq1,eq2->y);
      temp_3x3_eq2 = Scale(eq2,eq1->y*-1.0f);
      temp_2x2_eq1 = {temp_3x3_eq1.x + temp_3x3_eq2.x, temp_3x3_eq1.z + temp_3x3_eq2.z, temp_3x3_eq1.it + temp_3x3_eq2.it};
      temp_3x3_eq1 = Scale(eq1,eq3->y);
      temp_3x3_eq2 = Scale(eq3,eq1->y*-1.0f);
      temp_2x2_eq2 = {temp_3x3_eq1.x + temp_3x3_eq2.x, temp_3x3_eq1.z + temp_3x3_eq2.z,temp_3x3_eq1.it + temp_3x3_eq2.it};
      mode = 1;
    }else if(eq1->y == 0 || (eq2->y == 0 || eq3->y == 0)){
      temp_3x3_eq1 = Scale(eq1,eq2->z);
      temp_3x3_eq2 = Scale(eq2,eq1->z*-1.0f);
      temp_2x2_eq1 = {temp_3x3_eq1.x + temp_3x3_eq2.x, temp_3x3_eq1.y + temp_3x3_eq2.y, temp_3x3_eq1.it + temp_3x3_eq2.it};
      temp_3x3_eq1 = Scale(eq1,eq3->z);
      temp_3x3_eq2 = Scale(eq3,eq1->z*-1.0f);
      temp_2x2_eq2 = {temp_3x3_eq1.x + temp_3x3_eq2.x, temp_3x3_eq1.y + temp_3x3_eq2.y,temp_3x3_eq1.it + temp_3x3_eq2.it};
      mode = 2;
    }else{
      temp_3x3_eq1 = Scale(eq1,eq2->y);
      temp_3x3_eq2 = Scale(eq2,eq1->y*-1.0f);
      temp_2x2_eq1 = {temp_3x3_eq1.x + temp_3x3_eq2.x, temp_3x3_eq1.z + temp_3x3_eq2.z, temp_3x3_eq1.it + temp_3x3_eq2.it};
      temp_3x3_eq1 = Scale(eq1,eq3->y);
      temp_3x3_eq2 = Scale(eq3,eq1->y*-1.0f);
      temp_2x2_eq2 = {temp_3x3_eq1.x + temp_3x3_eq2.x, temp_3x3_eq1.z + temp_3x3_eq2.z,temp_3x3_eq1.it + temp_3x3_eq2.it};
      mode = 1;
    }
  }else{
    temp_3x3_eq1 = Scale(eq1,eq2->x);
    temp_3x3_eq2 = Scale(eq2,eq1->x*-1.0f);
    temp_2x2_eq1 = {temp_3x3_eq1.y + temp_3x3_eq2.y, temp_3x3_eq1.z + temp_3x3_eq2.z, temp_3x3_eq1.it + temp_3x3_eq2.it};
    temp_3x3_eq1 = Scale(eq1,eq3->x);
    temp_3x3_eq2 = Scale(eq3,eq1->x*-1.0f);
    temp_2x2_eq2 = {temp_3x3_eq1.y + temp_3x3_eq2.y, temp_3x3_eq1.z + temp_3x3_eq2.z, temp_3x3_eq1.it + temp_3x3_eq2.it};
    mode = 0;
  }
  if(debug){
    printf("eq1: %f %f %f\n",temp_2x2_eq1.x,temp_2x2_eq1.y,temp_2x2_eq1.it);
    printf("eq2: %f %f %f\n",temp_2x2_eq2.x,temp_2x2_eq2.y,temp_2x2_eq2.it);
  }
  res_2x2 = Resolve2x2(&temp_2x2_eq1,&temp_2x2_eq2);
  if(res_2x2.code == 0){
    switch(mode){
      case 0: //Resolve x
              res.eq.y = res_2x2.x;
              res.eq.z = res_2x2.y;
              if(eq1->x != 0){
                res.eq.x = (eq1->it - (eq1->y*res.eq.y) - (eq1->z*res.eq.z)) / eq1->x;
              }else if(eq2->x != 0){
                res.eq.x = (eq2->it - (eq2->y*res.eq.y) - (eq2->z*res.eq.z)) / eq2->x;
              }else if(eq3->x != 0){
                res.eq.x = (eq3->it - (eq3->y*res.eq.y) - (eq3->z*res.eq.z)) / eq3->x;
              }else{
                if(debug){
                  printf("All x are zero\n");
                }
                res.code = -1;
              }
              break;
      case 1: //Resolve y
              res.eq.x = res_2x2.x;
              res.eq.z = res_2x2.y;
              if(eq1->y != 0){
                res.eq.y = (eq1->it - (eq1->x*res.eq.x) - (eq1->z*res.eq.z)) / eq1->y;
              }else if(eq2->y != 0){
                res.eq.y = (eq2->it - (eq2->x*res.eq.x) - (eq2->z*res.eq.z)) / eq2->y;
              }else if(eq3->y != 0){
                res.eq.y = (eq3->it - (eq3->x*res.eq.x) - (eq3->z*res.eq.z)) / eq3->y;
              }else{
                if(debug){
                  printf("All y are zero\n");
                }
                res.code = -1;
              }
              break;
      case 2: //Resolve z
              res.eq.x = res_2x2.x;
              res.eq.y = res_2x2.y;
              if(eq1->z != 0){
                res.eq.z = (eq1->it - (eq1->x*res.eq.x) - (eq1->y*res.eq.y)) / eq1->z;
              }else if(eq2->z != 0){
                res.eq.z = (eq2->it - (eq2->x*res.eq.x) - (eq2->y*res.eq.y)) / eq2->z;
              }else if(eq3->z != 0){
                res.eq.z = (eq3->it - (eq3->x*res.eq.x) - (eq3->y*res.eq.y)) / eq3->z;
              }else{
                if(debug){
                  printf("All z are zero\n");
                }
                res.code = -1;
              }
              break;
    }
    res.code = 0;
    return res;
  }else{
    return {{0.0f,0.0f,0.0f},res_2x2.code};
  }
}

int main(){

  //Compatible 
  //TEq3x3 eq1 = {1.0f,-2.0f,3.0f,7.0f};
  //TEq3x3 eq2 = {-3.0f,1.0f,2.0f,-5.0f};
  //TEq3x3 eq3 = {2.0f,2.0f,1.0f,3.0f};

  //Compatible 
  //TEq3x3 eq1 = {0.0f,5.0f,0.0f,5.0f};
  //TEq3x3 eq2 = {2.0f,0.0f,4.0f,8.0f};
  //TEq3x3 eq3 = {1.0f,0.0f,-3.0f,-4.0f};

  //Compatible 
  //TEq3x3 eq1 = {2.0f,-3.0f,0.0f,6.0f};
  //TEq3x3 eq2 = {1.0f,4.0f,2.0f,8.0f};
  //TEq3x3 eq3 = {0.0f,2.0f,5.0f,5.0f};
  
  //Compatible 
  TEq3x3 eq1 = {2.0f,-3.0f,1.0f,4.0f};
  TEq3x3 eq2 = {1.0f,2.0f,-3.0f,-3.0f};
  TEq3x3 eq3 = {3.0f,-1.0f,4.0f,10.0f};

  //TEq3x3 eq1 = {0.0f,-1.0f,6.0f,7.0f};
  //TEq3x3 eq2 = {0.0f,1.0f,-2.0f,4.0f};
  //TEq3x3 eq3 = {0.0f,7.0f,3.0f,5.0f};

  //Incompatible
  //TEq3x3 eq1 = {2.0f,-1.0f,1.0f,-1.0f};
  //TEq3x3 eq2 = {4.0f,3.0f,5.0f,1.0f};
  //TEq3x3 eq3 = {0.0f,5.0f,3.0f,4.0f};

  //Compatible determinado
  //TEq3x3 eq1 = {1.0f,-3.0f,-4.0f,3.0f};
  //TEq3x3 eq2 = {3.0f,4.0f,-1.0f,13.0f};
  //TEq3x3 eq3 = {2.0f,-19.0f,-19.0f,2.0f};
  TRes3x3 res;
  if(eq1.x != 0){
    res = Resolve3x3(&eq1,&eq2,&eq3,true);
  }else if(eq2.x != 0){
    printf("se lanza esta\n");
    res = Resolve3x3(&eq2,&eq1,&eq3,true);
  }else if(eq3.x != 0){
    res = Resolve3x3(&eq3,&eq1,&eq2,true);
  }else{
    res = Resolve3x3(&eq1,&eq2,&eq3,true);
  }
  if(res.code == 0){
    printf("x: %f y: %f z:%f\n",res.eq.x,res.eq.y,res.eq.z);
  }else{
    //printf("cosas raras\n");
    if(res.code == 1){
      printf("Incompatible\n");
    }else if(res.code == 2){
      printf("Compatible determinado\n");
    }else{
      printf("%d\n",res.code);
    }
  }

  return 0;
}