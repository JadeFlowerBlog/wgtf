SUCCESS = 0
FAILURE = 1

GAME_RESOURCE_PATH = "game/res/fantasydemo"

BUILD_WIN32		= "WIN32"
BUILD_WIN64		= "WIN64"
BUILD_CONFIGS = [ BUILD_WIN32, BUILD_WIN64 ]

COLUMN_VALUE 			= "value"
COLUMN_TIMETORUN		= "timeToRun"
COLUMN_TOTMEMALLOC		= "totalMemoryAllocations"
COLUMN_PEAKALLOCBYTES	= "peakAllocatedBytes"
COLUMN_MEMLEAKEDCOUNT	= "memLeakedCount"
COLUMN_VALUES = [ COLUMN_VALUE, COLUMN_TIMETORUN, COLUMN_TOTMEMALLOC, COLUMN_PEAKALLOCBYTES, COLUMN_MEMLEAKEDCOUNT ]

EXE_DEBUG		= "DEBUG"
EXE_RELEASE		= "RELEASE"
EXE_CONSUMER 	= "CONSUMER"
EXE_TYPES = [ EXE_DEBUG, EXE_RELEASE, EXE_CONSUMER ]

EXE_DIRS = {
	BUILD_WIN32:  "game/bin/client/win32/",
	BUILD_WIN64:  "game/bin/client/win64/",
}

EXE_FILES = { 
	EXE_DEBUG:	  "bwclient_d.exe",
	EXE_RELEASE:  "bwclient_h.exe",
	EXE_CONSUMER: "bwclient.exe",
}

SUCCESS_STATE_OK = "OK"
SUCCESS_STATE_FAIL = "FAIL"
SUCCESS_STATES = [ SUCCESS_STATE_OK, SUCCESS_STATE_FAIL ]

TEST_FRAMERATE = "FRAMERATE"
TEST_LOADTIME = "LOADTIME"
TEST_PEAKMEM = "PEAKMEM"
TEST_TYPES = [ TEST_FRAMERATE, TEST_LOADTIME, TEST_PEAKMEM ]

DB_MEMORY = "MEMORY"
DB_SQLITE = "SQLITE"
DB_PRODUCTION = "MYSQL"
DB_TYPES = [ DB_MEMORY, DB_PRODUCTION, DB_SQLITE ]
DB_DEFAULT = DB_MEMORY

MYSQL_USER = 'perf'
MYSQL_PWD = 'p3rf'
MYSQL_HOST = 'db01'
MYSQL_DB = 'perf'

GRAPHITE_HOST = 'bwgraphite01'
GRAPHITE_PORT = '80'
GRAPHITE_STAT_PREFIX = ['stats', 'perf', 'bw', 'client']
GRAPHITE_STAT_PREFIX_FPS = GRAPHITE_STAT_PREFIX + ['fps']
GRAPHITE_STAT_PREFIX_LOADTIME = GRAPHITE_STAT_PREFIX + ['loadtime']
GRAPHITE_STAT_VALUE = 'value'
GRAPHITE_STAT_REFVALUE = 'refValue'
# 10 years specified since we can't tell Graphite we want stats
# from the dawn of time (or just all stats regardless of time).
GRAPHITE_TEN_YEARS_AGO = '-10y'

REPORT_EMAIL_ADDRESSES = [ "automation@bigworldtech.com" ]

