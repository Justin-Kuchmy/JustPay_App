#include <gtest/gtest.h>
#include "Helpers/PayrollCalculations.h"
#include "Models/DataObjects.h"
#define DEBUG_LOGS
#include "Utils/Log.h"

TEST(PayrollCalcTest, Applies_BothHalves_AlwaysTrue)
{
    int firstHalfOfMonth = 1;
    int secondHalfOfMonth = 2;
    EXPECT_TRUE(PayrollCalc::applies(DeductionSchedule::BothHalves, firstHalfOfMonth));
    EXPECT_TRUE(PayrollCalc::applies(DeductionSchedule::BothHalves, secondHalfOfMonth));
}

TEST(PayrollCalcTest, Applies_FirstHalf_OnlyMatchesHalfOne)
{
    int firstHalfOfMonth = 1;
    int secondHalfOfMonth = 2;
    EXPECT_TRUE(PayrollCalc::applies(DeductionSchedule::FirstHalf, firstHalfOfMonth));
    EXPECT_FALSE(PayrollCalc::applies(DeductionSchedule::FirstHalf, secondHalfOfMonth));
}

TEST(PayrollCalcTest, Applies_SecondHalf_OnlyMatchesHalfTwo)
{
    int firstHalfOfMonth = 1;
    int secondHalfOfMonth = 2;
    EXPECT_FALSE(PayrollCalc::applies(DeductionSchedule::SecondHalf, firstHalfOfMonth));
    EXPECT_TRUE(PayrollCalc::applies(DeductionSchedule::SecondHalf, secondHalfOfMonth));
}