cmake_minimum_required(VERSION 3.20)
project(sqlite3)

set (source
	"sqlite3.c"
	"sqlite3.h"
	)


add_library(sqlite3 ${source})