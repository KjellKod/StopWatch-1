#include "ToolsTestStopWatch.h"
#include "StopWatch.h"
#include "ThreadSafeStopWatch.h"
#include <chrono>
#include <thread>


TEST_F(ToolsTestStopWatch, UsSimple) {
   auto preStart = std::chrono::steady_clock::now();
   StopWatch watch{};
   auto elapsedUs = watch.ElapsedUs();
   auto preElapsedUs = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - preStart).count());

   EXPECT_TRUE(elapsedUs <= preElapsedUs) << "elapsedUs:" << elapsedUs << ", preElapsedUs:" << preElapsedUs;
}


TEST_F(ToolsTestStopWatch, UsSimpleAfterSleep) {
   auto preStart = std::chrono::steady_clock::now();
   StopWatch watch;
   std::this_thread::sleep_for(std::chrono::milliseconds(100));
   auto elapsedUs = watch.ElapsedUs();
   auto preElapsedUs = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - preStart).count());


   EXPECT_TRUE(elapsedUs <= preElapsedUs) << "elapsedUs:" << elapsedUs << ", preElapsedUs:" << preElapsedUs;
   EXPECT_TRUE(elapsedUs >= 1000 * 100);
}


TEST_F(ToolsTestStopWatch, MsSimpleAfterSleep) {
   auto preStart = std::chrono::steady_clock::now();
   StopWatch watch;
   std::this_thread::sleep_for(std::chrono::milliseconds(100));
   auto elapsedMs = watch.ElapsedMs();
   auto preElapsedMs = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - preStart).count());


   EXPECT_TRUE(elapsedMs <= preElapsedMs) << "elapsedUs:" << elapsedMs << ", preElapsedMs:" << preElapsedMs;
   EXPECT_TRUE(elapsedMs >= 100);
}
TEST_F(ToolsTestStopWatch, SecSimpleAfterSleep) {
   auto preStart = std::chrono::steady_clock::now();
   StopWatch watch;
   std::this_thread::sleep_for(std::chrono::seconds(1));
   auto elapsedSec = watch.ElapsedSec();
   auto preElapsedSec = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - preStart).count());


   EXPECT_TRUE(elapsedSec <= preElapsedSec) << "elapsedSec:" << elapsedSec << ", preElapsedSec:" << preElapsedSec;
   EXPECT_TRUE(elapsedSec >= 1);
}


TEST_F(ToolsTestStopWatch, Restart) {
   auto pre = std::chrono::steady_clock::now();
   StopWatch watch;
   auto restart = watch.Restart();
   EXPECT_TRUE(restart >= pre); // can be greater than previous

   std::this_thread::sleep_for(std::chrono::milliseconds(100));
   restart = watch.Restart();
   EXPECT_TRUE(restart > pre); // must be greater than previous
}


TEST_F(ToolsTestStopWatch, Comparisons) {
   StopWatch watch{};
   std::this_thread::sleep_for(std::chrono::seconds(1));
   auto elapsedNs = watch.ElapsedNs();
   auto elapsedUs = watch.ElapsedUs();
   auto elapsedMs = watch.ElapsedMs();
   auto elapsedSec = watch.ElapsedSec();

   // compare with itself
   EXPECT_TRUE(elapsedNs / 1000 <= elapsedUs) << "ns: " << elapsedUs << ". us: " << elapsedUs;
   EXPECT_TRUE(elapsedUs / 1000 <= elapsedMs) << "us: " << elapsedUs << ". ms: " << elapsedMs;
   EXPECT_TRUE(elapsedMs / 1000 <= elapsedSec);
   EXPECT_TRUE(elapsedUs >= 1000000);
   EXPECT_TRUE(elapsedMs >= 1000);
   EXPECT_TRUE(elapsedSec >= 1);
}

TEST_F(ToolsTestStopWatch, PrecisionComparisons) {
   PrecisionStopWatch watch{};
   std::this_thread::sleep_for(std::chrono::seconds(1));
   auto elapsedNs = watch.ElapsedNs();
   auto elapsedUs = watch.ElapsedUs();
   auto elapsedMs = watch.ElapsedMs();
   auto elapsedSec = watch.ElapsedSec();

   // compare with itself
   EXPECT_TRUE(elapsedNs / 1000 <= elapsedUs) << "ns: " << elapsedUs << ". us: " << elapsedUs;
   EXPECT_TRUE(elapsedUs / 1000 <= elapsedMs) << "us: " << elapsedUs << ". ms: " << elapsedMs;
   EXPECT_TRUE(elapsedMs / 1000 <= elapsedSec);
   EXPECT_TRUE(elapsedUs >= 1000000);
   EXPECT_TRUE(elapsedMs >= 1000);
   EXPECT_TRUE(elapsedSec >= 1);
}



TEST_F(ToolsTestStopWatch, ComparisonsWithOld) {
   StopWatch watch;
   auto oldStart = std::time(NULL);
   std::this_thread::sleep_for(std::chrono::seconds(1));
   auto oldElapsed = static_cast<uint64_t>(std::time(NULL) - oldStart);
   auto elapsedSec = watch.ElapsedSec();

   EXPECT_TRUE(elapsedSec >= oldElapsed);
   EXPECT_TRUE(elapsedSec >= 1);
   EXPECT_TRUE(oldElapsed >= 1);
}



/* ThreadSafeStopWatch tests*/
TEST_F(ToolsTestStopWatch, ThreadSafeSecSimpleAfterSleep) {
   ThreadSafeStopWatch threadSafeWatch;
   auto elapsedSec = threadSafeWatch.GetStopWatch().ElapsedSec();
   std::this_thread::sleep_for(std::chrono::seconds(1));
   elapsedSec = threadSafeWatch.GetStopWatch().ElapsedSec();
   EXPECT_TRUE(elapsedSec >= 1);
}

void StopWatchThread() {
   /* Start stop watch */
   ThreadSafeStopWatch threadSafeWatch;
   auto elapsedSec = threadSafeWatch.GetStopWatch().ElapsedSec();
   std::this_thread::sleep_for(std::chrono::seconds(5));
   elapsedSec = threadSafeWatch.GetStopWatch().ElapsedSec();
   EXPECT_TRUE(elapsedSec >= 5);
}

TEST_F(ToolsTestStopWatch, ThreadSafeSecBasicTest) {
   std::vector<std::thread*> threads;
   int threadCount = 5;
   for (int i = 0; i < threadCount; i++) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
      threads.push_back(new std::thread(StopWatchThread));
   }
   for (auto jt = threads.begin(); jt != threads.end(); jt++) {
      (*jt)->join();
      delete *jt;
   }
   threads.clear();
}
