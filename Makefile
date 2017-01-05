#/***************************************************************************************************
#* File Name:             Makefile 
#*                        
#* Application Developer: Rafael K. V. Maeda (HKUST), Zhe Wang (HKUST), 
#*                        Jiang Xu (HKUST) 
#*                        
#* Software:              COSMIC Heterogeneous Multiprocessor Benchmark 
#* Version:               2.0 
#* Author:                Rafael K. V. Maeda (HKUST), Wang Zhe (HKUST), Jiang Xu (HKUST), 
#*                        Zhehui Wang (HKUST), Peng Yang (HKUST), Luan H.K. Duong (HKUST), 
#*                        Zhifei Wang (HKUST), Hoaran Li (HKUST), Xuanqi Chen (HKUST), 
#*                        Zhongyuan Tian (HKUST) 
#*                        
#* Past Members:          Xiaowen Wu, Weichen Liu, Xuan Wang, Mahdi Nikdast 
#*                        
#* Website:               http://www.ece.ust.hk/~eexu 
#*                        
#* The copyright information of this software can be found in the file COPYRIGHT. 
#*
#**************************************************************************************************/
CC=g++
NAM="COSMIC benchmark suite tool - Mapping"
VER="v2.0"
DIRSRC=.
DIRHRD=$(DIRSRC)
DIROBJ=./build
DIRTGT=../bin
HDR=$(DIRHRD)/global_cfg.h $(DIRHRD)/app_edge.h $(DIRHRD)/app_task.h $(DIRHRD)/app_model.h $(DIRHRD)/arc_model.h\
 $(DIRHRD)/algo_buffer_alloc.h $(DIRHRD)/algo_mapping_sched.h $(DIRHRD)/result_writer.h
SRC=$(DIRSRC)/main.cpp $(DIRSRC)/global_cfg.cpp $(DIRSRC)/app_edge.cpp $(DIRSRC)/app_task.cpp $(DIRSRC)/app_model.cpp\
 $(DIRSRC)/arc_model.cpp $(DIRSRC)/algo_buffer_alloc.cpp $(DIRSRC)/algo_mapping_sched.cpp $(DIRSRC)/result_writer.cpp
OBJ=$(DIROBJ)/main.o $(DIROBJ)/global_cfg.o $(DIROBJ)/app_edge.o $(DIROBJ)/app_task.o $(DIROBJ)/app_model.o\
 $(DIROBJ)/arc_model.o $(DIROBJ)/algo_buffer_alloc.o $(DIROBJ)/algo_mapping_sched.o $(DIROBJ)/result_writer.o
TGT=$(DIRTGT)/cosmic_mapping.exec

all:$(TGT)

$(DIROBJ)/%.o: %.cpp
	@mkdir -p $(DIROBJ)
	@echo "Compiling "$<
	@$(CC) -c -o $@ $<

$(TGT): $(OBJ)
	@$(CC) -o $@ $^
	@echo "Linking "$(NAM)" "$(VER)" successfully finished!"

clean:
	@rm -f $(DIROBJ)/*
	@rm -f $(TGT)
