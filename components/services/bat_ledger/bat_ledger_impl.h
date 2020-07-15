/* Copyright (c) 2019 The Brave Authors. All rights reserved.
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this file,
 * You can obtain one at http://mozilla.org/MPL/2.0/. */

#ifndef BRAVE_COMPONENTS_SERVICES_BAT_LEDGER_BAT_LEDGER_IMPL_H_
#define BRAVE_COMPONENTS_SERVICES_BAT_LEDGER_BAT_LEDGER_IMPL_H_

#include <stdint.h>

#include <map>
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "base/memory/weak_ptr.h"
#include "bat/ledger/ledger.h"
#include "brave/components/services/bat_ledger/public/interfaces/bat_ledger.mojom.h"

namespace bat_ledger {

class BatLedgerClientMojoBridge;

class BatLedgerImpl :
    public mojom::BatLedger,
    public base::SupportsWeakPtr<BatLedgerImpl> {
 public:
  explicit BatLedgerImpl(
      mojo::PendingAssociatedRemote<mojom::BatLedgerClient> client_info);
  ~BatLedgerImpl() override;

  BatLedgerImpl(const BatLedgerImpl&) = delete;
  BatLedgerImpl& operator=(const BatLedgerImpl&) = delete;

  // bat_ledger::mojom::BatLedger
  void Initialize(
    const bool execute_create_script,
    InitializeCallback callback) override;
  void CreateWallet(CreateWalletCallback callback) override;
  void GetRewardsParameters(GetRewardsParametersCallback callback) override;

  void GetAutoContributeProperties(
      GetAutoContributePropertiesCallback callback) override;
  void GetPublisherMinVisitTime(
      GetPublisherMinVisitTimeCallback callback) override;
  void GetPublisherMinVisits(
      GetPublisherMinVisitsCallback callback) override;
  void GetPublisherAllowNonVerified(
      GetPublisherAllowNonVerifiedCallback callback) override;
  void GetPublisherAllowVideos(
      GetPublisherAllowVideosCallback callback) override;
  void GetAutoContributeEnabled(
      GetAutoContributeEnabledCallback callback) override;
  void GetReconcileStamp(GetReconcileStampCallback callback) override;

  void OnLoad(ledger::VisitDataPtr visit_data, uint64_t current_time) override;
  void OnUnload(uint32_t tab_id, uint64_t current_time) override;
  void OnShow(uint32_t tab_id, uint64_t current_time) override;
  void OnHide(uint32_t tab_id, uint64_t current_time) override;
  void OnForeground(uint32_t tab_id, uint64_t current_time) override;
  void OnBackground(uint32_t tab_id, uint64_t current_time) override;

  void OnPostData(const std::string& url,
      const std::string& first_party_url, const std::string& referrer,
      const std::string& post_data, ledger::VisitDataPtr visit_data) override;
  void OnXHRLoad(uint32_t tab_id, const std::string& url,
      const base::flat_map<std::string, std::string>& parts,
      const std::string& first_party_url, const std::string& referrer,
      ledger::VisitDataPtr visit_data) override;

  void SetPublisherExclude(
      const std::string& publisher_key,
      const ledger::PublisherExclude exclude,
      SetPublisherExcludeCallback callback) override;
  void RestorePublishers(RestorePublishersCallback callback) override;

  void FetchPromotions(FetchPromotionsCallback callback) override;
  void ClaimPromotion(
      const std::string& promotion_id,
      const std::string& payload,
      ClaimPromotionCallback callback) override;
  void AttestPromotion(
      const std::string& promotion_id,
      const std::string& solution,
      AttestPromotionCallback callback) override;
  void GetWalletPassphrase(GetWalletPassphraseCallback callback) override;
  void RecoverWallet(
      const std::string& pass_phrase,
      RecoverWalletCallback callback) override;

  void SetRewardsMainEnabled(bool enabled) override;
  void SetPublisherMinVisitTime(int duration_in_seconds) override;
  void SetPublisherMinVisits(int visits) override;
  void SetPublisherAllowNonVerified(bool allow) override;
  void SetPublisherAllowVideos(bool allow) override;
  void SetAutoContributionAmount(double amount) override;
  void SetAutoContributeEnabled(bool enabled) override;
  void UpdateAdsRewards() override;

  void OnTimer(uint32_t timer_id) override;

  void GetBalanceReport(ledger::ActivityMonth month, int32_t year,
      GetBalanceReportCallback callback) override;

  void IsWalletCreated(IsWalletCreatedCallback callback) override;

  void GetPublisherActivityFromUrl(
      uint64_t window_id,
      ledger::VisitDataPtr visit_data,
      const std::string& publisher_blob) override;

  void GetAutoContributionAmount(
      GetAutoContributionAmountCallback callback) override;
  void GetPublisherBanner(const std::string& publisher_id,
      GetPublisherBannerCallback callback) override;

  void OneTimeTip(
      const std::string& publisher_key,
      const double amount,
      OneTimeTipCallback callback) override;

  void RemoveRecurringTip(
      const std::string& publisher_key,
      RemoveRecurringTipCallback callback) override;
  void GetCreationStamp(GetCreationStampCallback callback) override;
  void GetRewardsMainEnabled(
      GetRewardsMainEnabledCallback callback) override;
  void HasSufficientBalanceToReconcile(
      HasSufficientBalanceToReconcileCallback callback) override;

  void GetTransactionHistory(
      GetTransactionHistoryCallback callback) override;
  void GetRewardsInternalsInfo(
      GetRewardsInternalsInfoCallback callback) override;
  void RefreshPublisher(
      const std::string& publisher_key,
      RefreshPublisherCallback callback) override;
  void StartMonthlyContribution() override;

  void SaveRecurringTip(
      ledger::RecurringTipPtr info,
      SaveRecurringTipCallback callback) override;

  void GetRecurringTips(GetRecurringTipsCallback callback) override;

  void GetOneTimeTips(GetOneTimeTipsCallback callback) override;

  void GetActivityInfoList(
    uint32_t start,
    uint32_t limit,
    ledger::ActivityInfoFilterPtr filter,
    GetActivityInfoListCallback callback) override;

  void GetExcludedList(GetExcludedListCallback callback) override;

  void SaveMediaInfo(
      const std::string& type,
      const base::flat_map<std::string, std::string>& args,
      SaveMediaInfoCallback callback) override;

  void SetInlineTippingPlatformEnabled(
      const ledger::InlineTipsPlatforms platform,
      bool enabled) override;

  void GetInlineTippingPlatformEnabled(
    const ledger::InlineTipsPlatforms platform,
    GetInlineTippingPlatformEnabledCallback callback) override;

  void GetShareURL(
    const std::string& type,
    const base::flat_map<std::string, std::string>& args,
    GetShareURLCallback callback) override;

  void GetPendingContributions(
    GetPendingContributionsCallback callback) override;

  void RemovePendingContribution(
      const uint64_t id,
      RemovePendingContributionCallback callback) override;

  void RemoveAllPendingContributions(
    RemovePendingContributionCallback callback) override;

  void GetPendingContributionsTotal(
    GetPendingContributionsTotalCallback callback) override;

  void FetchBalance(FetchBalanceCallback callback) override;

  void GetExternalWallet(const std::string& wallet_type,
                         GetExternalWalletCallback callback) override;

  void ExternalWalletAuthorization(
    const std::string& wallet_type,
    const base::flat_map<std::string, std::string>& args,
    ExternalWalletAuthorizationCallback callback) override;

  void DisconnectWallet(
    const std::string& wallet_type,
    DisconnectWalletCallback callback) override;

  void GetAnonWalletStatus(GetAnonWalletStatusCallback callback) override;

  void GetTransactionReport(
      const ledger::ActivityMonth month,
      const int year,
      GetTransactionReportCallback callback) override;

  void GetContributionReport(
      const ledger::ActivityMonth month,
      const int year,
      GetContributionReportCallback callback) override;

  void GetAllContributions(GetAllContributionsCallback callback) override;

  void SavePublisherInfo(
      ledger::PublisherInfoPtr info,
      SavePublisherInfoCallback callback) override;

  void GetMonthlyReport(
      const ledger::ActivityMonth month,
      const int year,
      GetMonthlyReportCallback callback) override;

  void GetAllMonthlyReportIds(
      GetAllMonthlyReportIdsCallback callback) override;

  void GetAllPromotions(GetAllPromotionsCallback callback) override;

  void Shutdown(ShutdownCallback callback) override;

 private:
  void SetCatalogIssuers(
      const std::string& info) override;

  void ConfirmAd(
      const std::string& json,
      const std::string& confirmation_type) override;
  void ConfirmAction(
      const std::string& creative_instance_id,
      const std::string& creative_set_id,
      const std::string& confirmation_type) override;

  // workaround to pass base::OnceCallback into std::bind
  template <typename Callback>
    class CallbackHolder {
     public:
      CallbackHolder(base::WeakPtr<BatLedgerImpl> client,
          Callback callback)
        : client_(client),
        callback_(std::move(callback)) {}
      ~CallbackHolder() = default;
      bool is_valid() { return !!client_.get(); }
      Callback& get() { return callback_; }

     private:
      base::WeakPtr<BatLedgerImpl> client_;
      Callback callback_;
    };

  static void OnGetBalanceReport(
      CallbackHolder<GetBalanceReportCallback>* holder,
      const ledger::Result result,
      ledger::BalanceReportInfoPtr report_info);

  static void OnClaimPromotion(
      CallbackHolder<ClaimPromotionCallback>* holder,
      const ledger::Result result,
      const std::string& response);

  static void OnAttestPromotion(
      CallbackHolder<AttestPromotionCallback>* holder,
      const ledger::Result result,
      ledger::PromotionPtr promotion);

  static void OnCreateWallet(
      CallbackHolder<CreateWalletCallback>* holder,
      ledger::Result result);

  static void OnInitialize(
      CallbackHolder<InitializeCallback>* holder,
      ledger::Result result);

  static void OnRecoverWallet(
      CallbackHolder<RecoverWalletCallback>* holder,
      ledger::Result result);

  static void OnGetRewardsParameters(
      CallbackHolder<GetRewardsParametersCallback>* holder,
      ledger::RewardsParametersPtr properties);

  static void OnSetPublisherExclude(
      CallbackHolder<SetPublisherExcludeCallback>* holder,
      const ledger::Result result);

  static void OnRestorePublishers(
      CallbackHolder<SetPublisherExcludeCallback>* holder,
      const ledger::Result result);

  static void OnGetPublisherBanner(
      CallbackHolder<GetPublisherBannerCallback>* holder,
      ledger::PublisherBannerPtr banner);

  static void OnRemoveRecurringTip(
      CallbackHolder<RemoveRecurringTipCallback>* holder,
      const ledger::Result result);

  static void OnOneTimeTip(
      CallbackHolder<OneTimeTipCallback>* holder,
      const ledger::Result result);

  static void OnGetTransactionHistory(
      CallbackHolder<GetTransactionHistoryCallback>* holder,
      std::unique_ptr<ledger::TransactionsInfo> history);

  static void OnGetRewardsInternalsInfo(
      CallbackHolder<GetRewardsInternalsInfoCallback>* holder,
      ledger::RewardsInternalsInfoPtr info);

  static void OnSaveRecurringTip(
      CallbackHolder<SaveRecurringTipCallback>* holder,
      ledger::Result result);

  static void OnGetRecurringTips(
      CallbackHolder<GetRecurringTipsCallback>* holder,
      ledger::PublisherInfoList list);

  static void OnGetOneTimeTips(
      CallbackHolder<GetRecurringTipsCallback>* holder,
      ledger::PublisherInfoList list);
  static void OnRefreshPublisher(
      CallbackHolder<RefreshPublisherCallback>* holder,
      ledger::PublisherStatus status);

  static void OnGetActivityInfoList(
    CallbackHolder<GetActivityInfoListCallback>* holder,
    ledger::PublisherInfoList list);

  static void OnGetExcludedList(
      CallbackHolder<GetExcludedListCallback>* holder,
      ledger::PublisherInfoList list);

  static void OnSaveMediaInfoCallback(
    CallbackHolder<SaveMediaInfoCallback>* holder,
    ledger::Result result,
    ledger::PublisherInfoPtr info);

  static void OnGetPendingContributions(
    CallbackHolder<GetPendingContributionsCallback>* holder,
    ledger::PendingContributionInfoList list);

  static void OnRemovePendingContribution(
    CallbackHolder<RemovePendingContributionCallback>* holder,
    ledger::Result result);

  static void OnRemoveAllPendingContributions(
    CallbackHolder<RemovePendingContributionCallback>* holder,
    ledger::Result result);

  static void OnGetPendingContributionsTotal(
    CallbackHolder<GetPendingContributionsTotalCallback>* holder,
    double amount);

  static void OnFetchPromotions(
    CallbackHolder<FetchPromotionsCallback>* holder,
    const ledger::Result result,
    ledger::PromotionList promotions);

  static void OnHasSufficientBalanceToReconcile(
    CallbackHolder<HasSufficientBalanceToReconcileCallback>* holder,
    bool sufficient);

  static void OnFetchBalance(
      CallbackHolder<FetchBalanceCallback>* holder,
      ledger::Result result,
      ledger::BalancePtr balance);

  static void OnGetExternalWallet(
    CallbackHolder<GetExternalWalletCallback>* holder,
    ledger::Result result,
    ledger::ExternalWalletPtr wallet);

  static void OnExternalWalletAuthorization(
    CallbackHolder<ExternalWalletAuthorizationCallback>* holder,
    ledger::Result result,
    const std::map<std::string, std::string>& args);

  static void OnDisconnectWallet(
    CallbackHolder<DisconnectWalletCallback>* holder,
    ledger::Result result);

  static void OnGetAnonWalletStatus(
      CallbackHolder<GetAnonWalletStatusCallback>* holder,
      const ledger::Result result);

  static void OnGetTransactionReport(
      CallbackHolder<GetTransactionReportCallback>* holder,
      ledger::TransactionReportInfoList list);

  static void OnGetContributionReport(
      CallbackHolder<GetContributionReportCallback>* holder,
      ledger::ContributionReportInfoList list);

  static void OnGetAllContributions(
      CallbackHolder<GetAllContributionsCallback>* holder,
      ledger::ContributionInfoList list);

  static void OnSavePublisherInfo(
      CallbackHolder<SavePublisherInfoCallback>* holder,
      const ledger::Result result);

  static void OnGetMonthlyReport(
      CallbackHolder<GetMonthlyReportCallback>* holder,
      const ledger::Result result,
      ledger::MonthlyReportInfoPtr info);

  static void OnGetAllMonthlyReportIds(
      CallbackHolder<GetAllMonthlyReportIdsCallback>* holder,
      const std::vector<std::string>& ids);

  static void OnGetAllPromotions(
      CallbackHolder<GetAllPromotionsCallback>* holder,
      ledger::PromotionMap items);

  static void OnShutdown(
      CallbackHolder<ShutdownCallback>* holder,
      const ledger::Result result);

  std::unique_ptr<BatLedgerClientMojoBridge> bat_ledger_client_mojo_bridge_;
  std::unique_ptr<ledger::Ledger> ledger_;
};

}  // namespace bat_ledger

#endif  // BRAVE_COMPONENTS_SERVICES_BAT_LEDGER_BAT_LEDGER_IMPL_H_
