/* Copyright (c) 2020 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#include "bat/ads/internal/ad_targeting/processors/text_classification/text_classification_processor.h"

#include <vector>

#include "bat/ads/internal/ad_serving/ad_targeting/models/text_classification/text_classification_model.h"
#include "bat/ads/internal/unittest_base.h"
#include "bat/ads/internal/unittest_util.h"

// npm run test -- brave_unit_tests --filter=BatAds*

namespace ads {
namespace ad_targeting {

namespace {
const char kEnLanguageCodeId[] = "emgmepnebbddgnkhfmhdhmjifkglkamo";
}  // namespace

class BatAdsTextClassificationProcessorTest : public UnitTestBase {
 protected:
  BatAdsTextClassificationProcessorTest() = default;

  ~BatAdsTextClassificationProcessorTest() override = default;
};

TEST_F(BatAdsTextClassificationProcessorTest,
    DoNotPersistProbabilitiesHistoryForUntargetedLocale) {
  // Arrange
  resource::TextClassification resource;
  resource.LoadForLocale("ja-JP");

  // Act
  const std::string text = "一部のコンテンツ";
  processor::TextClassification processor(&resource);
  processor.Process(text);

  // Assert
  const TextClassificationProbabilitiesList list =
      Client::Get()->GetTextClassificationProbabilitiesHistory();

  EXPECT_TRUE(list.empty());
}

TEST_F(BatAdsTextClassificationProcessorTest,
    DoNotPersistProbabilitiesHistoryForEmptyText) {
  // Arrange
  resource::TextClassification resource;
  resource.LoadForLocale("en-US");

  // Act
  const std::string text = "";
  processor::TextClassification processor(&resource);
  processor.Process(text);

  // Assert
  const TextClassificationProbabilitiesList list =
      Client::Get()->GetTextClassificationProbabilitiesHistory();

  EXPECT_TRUE(list.empty());
}

TEST_F(BatAdsTextClassificationProcessorTest,
    DoNotPersistProbabilitiesHistoryIfTextHasNeverBeenProcessed) {
  // Arrange
  resource::TextClassification resource;
  resource.LoadForLocale("en-US");

  // Act
  model::TextClassification model;
  const SegmentList segments = model.GetSegments();

  // Assert
  const TextClassificationProbabilitiesList list =
      Client::Get()->GetTextClassificationProbabilitiesHistory();

  EXPECT_TRUE(list.empty());
}

TEST_F(BatAdsTextClassificationProcessorTest,
    PersistProbabilitiesHistoryForText) {
  // Arrange
  resource::TextClassification resource;
  resource.LoadForLocale("en-US");

  // Act
  const std::string text = "Some content about technology & computing";
  processor::TextClassification processor(&resource);
  processor.Process(text);

  // Assert
  const TextClassificationProbabilitiesList list =
      Client::Get()->GetTextClassificationProbabilitiesHistory();

  EXPECT_EQ(1UL, list.size());
}

TEST_F(BatAdsTextClassificationProcessorTest,
    PersistProbabilitiesHistoryForMultipleText) {
  // Arrange
  resource::TextClassification resource;
  resource.LoadForId(kEnLanguageCodeId);

  // Act
  const std::vector<std::string> texts = {
    "Some content about cooking food",
    "Some content about finance & banking",
    "Some content about technology & computing"
  };

  processor::TextClassification processor(&resource);
  for (const auto& text : texts) {
    processor.Process(text);
  }

  // Assert
  const TextClassificationProbabilitiesList list =
      Client::Get()->GetTextClassificationProbabilitiesHistory();

  EXPECT_EQ(3UL, list.size());
}

}  // namespace ad_targeting
}  // namespace ads
