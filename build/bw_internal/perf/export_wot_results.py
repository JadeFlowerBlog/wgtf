import sys
import performance_test.export

from performance_test.database import WotPerformanceTestResult

performance_test.export.EXPORT_TYPE = WotPerformanceTestResult
if __name__ == "__main__":
	sys.exit( performance_test.export.main() )
