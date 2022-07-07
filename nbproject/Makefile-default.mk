#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=mkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/firmware_light_for_philippines.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=${DISTDIR}/firmware_light_for_philippines.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=CS.c Feedback.c HW.c HW_Init.c ISRs.c main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/CS.o ${OBJECTDIR}/Feedback.o ${OBJECTDIR}/HW.o ${OBJECTDIR}/HW_Init.o ${OBJECTDIR}/ISRs.o ${OBJECTDIR}/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/CS.o.d ${OBJECTDIR}/Feedback.o.d ${OBJECTDIR}/HW.o.d ${OBJECTDIR}/HW_Init.o.d ${OBJECTDIR}/ISRs.o.d ${OBJECTDIR}/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/CS.o ${OBJECTDIR}/Feedback.o ${OBJECTDIR}/HW.o ${OBJECTDIR}/HW_Init.o ${OBJECTDIR}/ISRs.o ${OBJECTDIR}/main.o

# Source Files
SOURCEFILES=CS.c Feedback.c HW.c HW_Init.c ISRs.c main.c



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-default.mk ${DISTDIR}/firmware_light_for_philippines.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATtiny816
# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/CS.o: CS.c  .generated_files/flags/default/5ff2885ed992abb0817ba28952f5b2749b6380e7 .generated_files/flags/default/9f5b7f78d942c6e69c552590a23a348205616a40
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CS.o.d 
	@${RM} ${OBJECTDIR}/CS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/CS.o.d" -MT "${OBJECTDIR}/CS.o.d" -MT ${OBJECTDIR}/CS.o -o ${OBJECTDIR}/CS.o CS.c 
	
${OBJECTDIR}/Feedback.o: Feedback.c  .generated_files/flags/default/ea96015a458e9c8bd5d4c965b1b8027906450fbb .generated_files/flags/default/9f5b7f78d942c6e69c552590a23a348205616a40
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Feedback.o.d 
	@${RM} ${OBJECTDIR}/Feedback.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/Feedback.o.d" -MT "${OBJECTDIR}/Feedback.o.d" -MT ${OBJECTDIR}/Feedback.o -o ${OBJECTDIR}/Feedback.o Feedback.c 
	
${OBJECTDIR}/HW.o: HW.c  .generated_files/flags/default/b692c4743260b64c8e9a924beeccc512128451ae .generated_files/flags/default/9f5b7f78d942c6e69c552590a23a348205616a40
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/HW.o.d 
	@${RM} ${OBJECTDIR}/HW.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/HW.o.d" -MT "${OBJECTDIR}/HW.o.d" -MT ${OBJECTDIR}/HW.o -o ${OBJECTDIR}/HW.o HW.c 
	
${OBJECTDIR}/HW_Init.o: HW_Init.c  .generated_files/flags/default/1bd2fc0af73c48048cf89ecdbe60d119fb5b0dbd .generated_files/flags/default/9f5b7f78d942c6e69c552590a23a348205616a40
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/HW_Init.o.d 
	@${RM} ${OBJECTDIR}/HW_Init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/HW_Init.o.d" -MT "${OBJECTDIR}/HW_Init.o.d" -MT ${OBJECTDIR}/HW_Init.o -o ${OBJECTDIR}/HW_Init.o HW_Init.c 
	
${OBJECTDIR}/ISRs.o: ISRs.c  .generated_files/flags/default/c24993b800898c409e24d6274f4e2ebbced1ec5a .generated_files/flags/default/9f5b7f78d942c6e69c552590a23a348205616a40
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ISRs.o.d 
	@${RM} ${OBJECTDIR}/ISRs.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/ISRs.o.d" -MT "${OBJECTDIR}/ISRs.o.d" -MT ${OBJECTDIR}/ISRs.o -o ${OBJECTDIR}/ISRs.o ISRs.c 
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/3dbb34b834883e55c5e36ce9d6dca211e0c343fb .generated_files/flags/default/9f5b7f78d942c6e69c552590a23a348205616a40
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -D__DEBUG=1 -g -DDEBUG  -gdwarf-2  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/main.o.d" -MT "${OBJECTDIR}/main.o.d" -MT ${OBJECTDIR}/main.o -o ${OBJECTDIR}/main.o main.c 
	
else
${OBJECTDIR}/CS.o: CS.c  .generated_files/flags/default/6bcf0522508114b373c0c11550ddf3dbb9af332f .generated_files/flags/default/9f5b7f78d942c6e69c552590a23a348205616a40
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/CS.o.d 
	@${RM} ${OBJECTDIR}/CS.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/CS.o.d" -MT "${OBJECTDIR}/CS.o.d" -MT ${OBJECTDIR}/CS.o -o ${OBJECTDIR}/CS.o CS.c 
	
${OBJECTDIR}/Feedback.o: Feedback.c  .generated_files/flags/default/5dc517367766d0847f13bbb25952593939474bb7 .generated_files/flags/default/9f5b7f78d942c6e69c552590a23a348205616a40
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/Feedback.o.d 
	@${RM} ${OBJECTDIR}/Feedback.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/Feedback.o.d" -MT "${OBJECTDIR}/Feedback.o.d" -MT ${OBJECTDIR}/Feedback.o -o ${OBJECTDIR}/Feedback.o Feedback.c 
	
${OBJECTDIR}/HW.o: HW.c  .generated_files/flags/default/b74b75a76b5f5941be59d436d11a108179ed3bc .generated_files/flags/default/9f5b7f78d942c6e69c552590a23a348205616a40
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/HW.o.d 
	@${RM} ${OBJECTDIR}/HW.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/HW.o.d" -MT "${OBJECTDIR}/HW.o.d" -MT ${OBJECTDIR}/HW.o -o ${OBJECTDIR}/HW.o HW.c 
	
${OBJECTDIR}/HW_Init.o: HW_Init.c  .generated_files/flags/default/4b24d90132820a2efe342e1b07aad6d079092269 .generated_files/flags/default/9f5b7f78d942c6e69c552590a23a348205616a40
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/HW_Init.o.d 
	@${RM} ${OBJECTDIR}/HW_Init.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/HW_Init.o.d" -MT "${OBJECTDIR}/HW_Init.o.d" -MT ${OBJECTDIR}/HW_Init.o -o ${OBJECTDIR}/HW_Init.o HW_Init.c 
	
${OBJECTDIR}/ISRs.o: ISRs.c  .generated_files/flags/default/acf6b24aeaa5ad84b4978138ac7c899a4cc7e6cc .generated_files/flags/default/9f5b7f78d942c6e69c552590a23a348205616a40
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/ISRs.o.d 
	@${RM} ${OBJECTDIR}/ISRs.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/ISRs.o.d" -MT "${OBJECTDIR}/ISRs.o.d" -MT ${OBJECTDIR}/ISRs.o -o ${OBJECTDIR}/ISRs.o ISRs.c 
	
${OBJECTDIR}/main.o: main.c  .generated_files/flags/default/b0ed99f9deaf7bcd7d9ea1087ce3fda329757dba .generated_files/flags/default/9f5b7f78d942c6e69c552590a23a348205616a40
	@${MKDIR} "${OBJECTDIR}" 
	@${RM} ${OBJECTDIR}/main.o.d 
	@${RM} ${OBJECTDIR}/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -c  -x c -D__$(MP_PROCESSOR_OPTION)__   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -DXPRJ_default=$(CND_CONF)  $(COMPARISON_BUILD)  -gdwarf-3     -MD -MP -MF "${OBJECTDIR}/main.o.d" -MT "${OBJECTDIR}/main.o.d" -MT ${OBJECTDIR}/main.o -o ${OBJECTDIR}/main.o main.c 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${DISTDIR}/firmware_light_for_philippines.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/firmware_light_for_philippines.X.${IMAGE_TYPE}.map  -D__DEBUG=1  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"   -gdwarf-2 -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -gdwarf-3     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/firmware_light_for_philippines.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o ${DISTDIR}/firmware_light_for_philippines.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group  -Wl,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1
	@${RM} ${DISTDIR}/firmware_light_for_philippines.X.${IMAGE_TYPE}.hex 
	
else
${DISTDIR}/firmware_light_for_philippines.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} ${DISTDIR} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -mcpu=$(MP_PROCESSOR_OPTION) -Wl,-Map=${DISTDIR}/firmware_light_for_philippines.X.${IMAGE_TYPE}.map  -DXPRJ_default=$(CND_CONF)  -Wl,--defsym=__MPLAB_BUILD=1   -mdfp="${DFP_DIR}/xc8"  -Wl,--gc-sections -O1 -ffunction-sections -fdata-sections -fshort-enums -fno-common -funsigned-char -funsigned-bitfields -Wall -gdwarf-3     $(COMPARISON_BUILD) -Wl,--memorysummary,${DISTDIR}/memoryfile.xml -o ${DISTDIR}/firmware_light_for_philippines.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  -o ${DISTDIR}/firmware_light_for_philippines.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}      -Wl,--start-group  -Wl,-lm -Wl,--end-group 
	${MP_CC_DIR}/avr-objcopy -O ihex "${DISTDIR}/firmware_light_for_philippines.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}" "${DISTDIR}/firmware_light_for_philippines.X.${IMAGE_TYPE}.hex"
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${OBJECTDIR}
	${RM} -r ${DISTDIR}

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell "${PATH_TO_IDE_BIN}"mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
