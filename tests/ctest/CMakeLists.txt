enable_testing()

set(RUNNER_FD "${CMAKE_CURRENT_SOURCE_DIR}/TESTFD.bin")
set(ROMDIR "${CMAKE_CURRENT_SOURCE_DIR}/../../CompROM")
set(BOOT_CD "${CMAKE_CURRENT_SOURCE_DIR}/../../resources/CDIMG.iso")

add_test(EGB_4EH_CLOSEPAINT
	Tsugaru_CUI
		${ROMDIR}
		-FD0
		${RUNNER_FD}
		-BOOTKEY
		F0
		-SHAREDDIR
		"${CMAKE_CURRENT_SOURCE_DIR}/EGB_CLOSEPAINT"
		-DEBUG
		-UNITTEST
		-USEFPU
)

add_test(EGB_25H_PUTBLOCK___EGB_23H_PUTBLOCKCOLOR
	Tsugaru_CUI
		${ROMDIR}
		-FD0
		${RUNNER_FD}
		-BOOTKEY
		F0
		-SHAREDDIR
		"${CMAKE_CURRENT_SOURCE_DIR}/EGB_PUTBLOCK"
		-DEBUG
		-UNITTEST
		-USEFPU
)

add_test(SND_25H_PCM_PLAY2
	Tsugaru_CUI
		${ROMDIR}
		-FD0
		${RUNNER_FD}
		-BOOTKEY
		F0
		-SHAREDDIR
		"${CMAKE_CURRENT_SOURCE_DIR}/SND_PCM_PLAY2"
		-DEBUG
		-UNITTEST
		-USEFPU
)

add_test(TACTICAL_AIR_WING_OPENING_DEMO
	Tsugaru_CUI
		${ROMDIR}
		-FD0
		${RUNNER_FD}
		-BOOTKEY
		F0
		-SHAREDDIR
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../TOWNSEMU_TEST/FREEWARE/AIRWING"
		-DEBUG
		-UNITTEST
		-POWEROFFAT 000C:00055526
)

add_test(PLANET_ATTACKER_OPENING
	Tsugaru_CUI
		${ROMDIR}
		-FD0
		${RUNNER_FD}
		-BOOTKEY
		F0
		-SHAREDDIR
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../TOWNSEMU_TEST/FREEWARE/PATTACK"
		-DEBUG
		-UNITTEST
		-POWEROFFAT 000C:00062DC4
)

add_test(GROUND_ATTACK_TITLE
	Tsugaru_CUI
		${ROMDIR}
		-FD0
		${RUNNER_FD}
		-BOOTKEY
		F0
		-SHAREDDIR
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../TOWNSEMU_TEST/FREEWARE/GATTACK"
		-DEBUG
		-UNITTEST
		-POWEROFFAT 000C:0000A4F2
)
add_test(SWORD_EDGE_AUTO_DEMO
	Tsugaru_CUI
		${ROMDIR}
		-FD0
		${RUNNER_FD}
		-BOOTKEY
		F0
		-SHAREDDIR
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../TOWNSEMU_TEST/FREEWARE/SWDEDGE"
		-DEBUG
		-UNITTEST
		-POWEROFFAT 000C:0000183A 
)
add_test(JSOUND_START
	Tsugaru_CUI
		${ROMDIR}
		-FD0
		${RUNNER_FD}
		-BOOTKEY
		F0
		-SHAREDDIR
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../TOWNSEMU_TEST/FREEWARE/JSOUND"
		-DEBUG
		-UNITTEST
		-POWEROFFAT 000C:00054517
)

add_test(PANIC_BALL_2_START
	Tsugaru_CUI
		${ROMDIR}
		-FD0
		${RUNNER_FD}
		-BOOTKEY
		F0
		-SHAREDDIR
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../TOWNSEMU_TEST/FREEWARE/PANIC2"
		-DEBUG
		-UNITTEST
		-POWEROFFAT 000C:000051EB
)

add_test(SKYDUEL_START
	Tsugaru_CUI
		${ROMDIR}
		-FD0
		${RUNNER_FD}
		-BOOTKEY
		F0
		-SHAREDDIR
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../TOWNSEMU_TEST/FREEWARE/SKYDUEL"
		-DEBUG
		-UNITTEST
		-POWEROFFAT 000C:000018B9
)

add_test(ALLTYNEX_START
	Tsugaru_CUI
		${ROMDIR}
		-FD0
		${RUNNER_FD}
		-BOOTKEY
		F0
		-SHAREDDIR
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../TOWNSEMU_TEST/FREEWARE/ALLTYNEX"
		-EVTLOG
		"${CMAKE_CURRENT_SOURCE_DIR}/ALLTYNEX.evt"
		-DEBUG
		-UNITTEST
		-POWEROFFAT	000C:0002B4B7
)

add_test(VSGP_GAME_START
	Tsugaru_CUI
		${ROMDIR}
		-FD0
		${RUNNER_FD}
		-BOOTKEY
		F0
		-SHAREDDIR
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../TOWNSEMU_TEST/FREEWARE/VSGP"
		-EVTLOG
		"${CMAKE_CURRENT_SOURCE_DIR}/VSGP.evt"
		-DEBUG
		-UNITTEST
		-POWEROFFAT 000C:00005DA1 
)

add_test(VIPS2_START
	Tsugaru_CUI
		${ROMDIR}
		-FD0
		${RUNNER_FD}
		-BOOTKEY
		F0
		-SHAREDDIR
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../TOWNSEMU_TEST/FREEWARE/VIPS2"
		-DEBUG
		-UNITTEST
		-POWEROFFAT 000C:000376F8
)

add_test(IRONFIST_REVISED_EDITION_DEMO_START
	Tsugaru_CUI
		${ROMDIR}
		-FD0
		${RUNNER_FD}
		-CD
		${BOOT_CD}
		-BOOTKEY
		F0
		-SHAREDDIR
		"${CMAKE_CURRENT_SOURCE_DIR}/../../../TOWNSEMU_TEST/FREEWARE/IRONFIST"
		-DEBUG
		-UNITTEST
		-POWEROFFAT 000C:0004DF69
)
