#ifndef REAL_NUM_H
#define REAL_NUM_H
struct realnum{
	int32_t value;
};
struct realnum realnum_init(int32_t v);
int32_t trunc(struct realnum x);
int32_t round(struct realnum x);
struct realnum realnum_add(struct realnum x,struct realnum y);
struct realnum realnum_addint(struct realnum x,int32_t y);
struct realnum realnum_sub(struct realnum x,struct realnum y);
struct realnum realnum_subint(struct realnum x,int32_t y);
struct realnum realnum_mul(struct realnum x,struct realnum y);
struct realnum realnum_mulint(struct realnum x,int32_t y);
struct realnum realnum_div(struct realnum x,struct realnum y);
struct realnum realnum_divint(struct realnum x,int32_t y);
struct realnum div_int(int32_t x,int32_t y);
/* 
struct realnum realnum_init(int32_t v){
	struct realnum a;
	a.value=v;
	return a;
}
int32_t trunc(struct realnum x){
	return x.value/W;
}
int32_t round(struct realnum x){
	return x.value/W+(x.value%W>=W/2?1:0);
}
struct realnum realnum_add(struct realnum x,struct realnum y){
	return realnum_init(x.value+y.value);
}
struct realnum realnum_addint(struct realnum x,int32_t y){
	return realnum_init(x.value+y*W);
}
struct realnum realnum_sub(struct realnum x,struct realnum y){
	return realnum_init(x.value-y.value);
}
struct realnum realnum_subint(struct realnum x,int32_t y){
	return realnum_init(x.value-y*W);
}

struct realnum realnum_mul(struct realnum x,struct realnum y){
	return realnum_init((int64_t)x.value*y.value/W);
}
struct realnum realnum_mulint(struct realnum x,int32_t y){
	return realnum_init(x.value*y);
}
struct realnum realnum_div(struct realnum x,struct realnum y){
	return realnum_init((int64_t)x.value*W/y.value);
}
struct realnum realnum_divint(struct realnum x,int32_t y){
	return realnum_init(x.value/y);
}
struct realnum div_int(int32_t x,int32_t y){
	return realnum_init((int64_t)x*W/y);
}*/
#endif
