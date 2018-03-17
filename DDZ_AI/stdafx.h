#ifndef STDAFX_H
#define STDAFX_H

#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include "log.h"
#include "random.h"
#include "common.h"

typedef std::vector<uint8_t> CardVector;

extern Random CommonRandom;
extern std::default_random_engine DefaultRandomEngine;

#define VECTOR_COPY(src,des)    des.resize(src.size());\
    std::copy(src.begin(), src.end(), des.begin())

#define VECTOR_MIN_INDEX(return_variable_name,src)auto  minIndex = std::min_element(src.begin(), src.end());\
size_t return_variable_name = minIndex - src.begin()

#define VECTOR_MAX_INDEX(return_variable_name,src)auto  maxIndex = std::max_element(src.begin(), src.end());\
size_t return_variable_name = maxIndex - src.begin()

#define VECTOR_MIN_VALUE(return_variable_name,src)size_t  min = *std::min_element(src.begin(), src.end())

#define VECTOR_MAX_VALUE(return_variable_name,src)size_t  max = *std::max_element(src.begin(), src.end())

#define VECTOR_INSERT_UNIQUE(vector_variable,same_object)auto itFinder = std::find(vector_variable.begin(), vector_variable.end(), same_object);\
if (vector_variable.size() == 0 || itFinder == vector_variable.end()) {\
    vector_variable.push_back(std::move(same_object));\
}
#ifdef __linux__
#define  _ASSERT(x) Q_ASSERT(x)
#endif

#if USE_LOG4CPP
#define DEBUG_LOG(text) Log::InfoF(text)
#else
#include <QDebug>
#define DEBUG_LOG(text) qDebug()<<(QString::fromStdString( text));
#endif

#endif // STDAFX_H