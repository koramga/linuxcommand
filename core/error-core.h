#ifndef __ERROR_CORE_H__
#define __ERROR_CORE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __ERR_MSG_SIZE__ 1024

#define ERR_INDEX -1            
//인덱스 오류, 인덱스를 못찾을때 발생
#define ERR_CREATE -2
//생성 오류
#define ERR_OVERFLOW -3
//꽉 찼을때 생기는 오류
#define ERR_INIT 0
//에러 초기값
#define ERR_NONE 1
//에러 없음(성공적임)

void setErrorMsssage(const char* errorMessage);
void setErrorNumber(const int errNum);
void setError(const int errNum, const char* errorMessage);
const char* getErrorMessage();
const int getErrorNumber();
void renderErrorMessage();

#endif
