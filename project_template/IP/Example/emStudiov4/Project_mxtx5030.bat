@ECHO OFF
SET TARGET_CHIP=MXTX5030
SET PACK_FILE=..\..\..\..\utilities\pack\Holtek.HT32_DFP.latest.pack

start Project_mxtx5030.emProject

REM pyocd erase --chip -t %TARGET_CHIP% --pack=%PACK_FILE%
pyocd gdbserver -t %TARGET_CHIP% --pack=%PACK_FILE%