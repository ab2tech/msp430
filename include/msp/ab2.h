// Copyright (C) [2012, 2013] [AB2 Technologies] [Austin Beam, Alan Bullick]
//
//    Licensed under the Apache License, Version 2.0 (the "License");
//    you may not use this file except in compliance with the License.
//    You may obtain a copy of the License at
//
//        http://www.apache.org/licenses/LICENSE-2.0
//
//    Unless required by applicable law or agreed to in writing, software
//    distributed under the License is distributed on an "AS IS" BASIS,
//    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//    See the License for the specific language governing permissions and
//    limitations under the License.

// Centralize inclusions made for every project and file and also create
// definitions which are used globally

// This file should be included in all AB2 projects, and additions here should
// be 100% applicable to all projects (common definitions, etc)

#pragma once

// Include universally required files here
#include <stdint.h>
#include "macros.h"

// Define common frequencies for use with F_CPU
#define F_1MHz   1000000UL
#define F_4MHz   4000000UL
#define F_8MHz   8000000UL
#define F_16MHz 16000000UL
#define F_20MHz 20000000UL
#define F_25MHz 25000000UL

// Define a boolean type that can be used in all AB2 software
typedef enum
{
  FALSE=0,
  TRUE=1
} bool_t;
