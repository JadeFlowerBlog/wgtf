
OK_STATE		= "OK"
FAIL_STAT		= "FAIL"
SUCCESS_STATE = [ OK_STATE, FAIL_STAT ]

BUILD_WIN32		= "WIN32"
BUILD_WIN64		= "WIN64"
BUILD_CONFIGS = [ BUILD_WIN32, BUILD_WIN64 ]

EXE_DEBUG		= "Debug"
EXE_RELEASE		= "Hybrid"
EXE_CONSUMER 	= "Consumer_Release"
EXE_TYPES = [ EXE_DEBUG, EXE_RELEASE, EXE_CONSUMER ]

DB_MEMORY = "MEMORY"
DB_PRODUCTION = "MYSQL"
DB_TYPES = [ DB_MEMORY, DB_PRODUCTION ]
DB_DEFAULT = DB_MEMORY

MYSQL_USER = 'perf'
MYSQL_PWD = 'p3rf'
MYSQL_HOST = 'db01'
MYSQL_DB = 'perf'

GRAPHITE_HOST = 'bwgraphite01'
GRAPHITE_PORT = '80'
GRAPHITE_STAT_PREFIX_WOT = ['stats', 'perf', 'wot', 'client']
GRAPHITE_STAT_PREFIX = ['stats', 'perf', 'bw', 'client']
GRAPHITE_STAT_PREFIX_SMOKE = GRAPHITE_STAT_PREFIX + ['smoke']
GRAPHITE_STAT_VALUE = 'value'
GRAPHITE_STAT_REFVALUE = 'refValue'
GRAPHITE_STAT_TIMETORUN = 'timeToRun'
GRAPHITE_STAT_LOADTIME = 'loadTimeSec'
GRAPHITE_STAT_FRAMETIME = 'frameTimeMs'
GRAPHITE_STAT_FPS = 'fps'
GRAPHITE_STAT_PEAKALLOCBYTES = 'peakAllocatedBytes'
GRAPHITE_STAT_TOTALMEMALLOCS = 'totalMemoryAllocations'
GRAPHITE_STAT_SUCCESSSTATE = 'successState'
GRAPHITE_STAT_REPLAYNAME = 'replayName'
GRAPHITE_STAT_MEMLEAKCOUNT = 'memLeakedCount'
# 10 years specified since we can't tell Graphite we want stats
# from the dawn of time (or just all stats regardless of time).
GRAPHITE_TEN_YEARS_AGO = '-10y'