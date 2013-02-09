// Copyright (C) 2012, 2013
// AB2 Technologies
// Austin Beam, Alan Bullick

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

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
