// Copyright (c) 2025, Agibot Co., Ltd.
// AGILINK OmniHand SDK is licensed under Mulan PSL v2.

#include <gtest/gtest.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "nlohmann/json.hpp"
#include "omnihand_server/service/hand_service.h"
#include "omnihand_server/adapter/hand_device.h"
#include "omnihand_server/protocol/json_codec.h"

using agilink::omnihand::server::ApplicationError;
using agilink::omnihand::server::Capabilities;
using agilink::omnihand::server::HandService;
using agilink::omnihand::server::HandCreateSpec;
using agilink::omnihand::server::IHandDevice;
using agilink::omnihand::server::IHandDeviceFactory;
using agilink::omnihand::server::MakeProblem;
using agilink::omnihand::server::ProductInfo;
using agilink::omnihand::server::protocol::DataValue;
using agilink::omnihand::server::protocol::DataValueFromJson;
using agilink::omnihand::server::protocol::DataValueToJson;
using Json = nlohmann::json;

namespace {

class FakeHandDevice final : public IHandDevice {
 public:
  explicit FakeHandDevice(std::string product_type) : product_type_(std::move(product_type)) {}

  ProductInfo Descriptor() const override {
    return ProductInfo{product_type_, "Fake " + product_type_, 3, true};
  }

  Capabilities GetCapabilities() const override {
    Capabilities capabilities;
    capabilities.supports_tactile = true;
    capabilities.supports_joint_position_cmd = true;
    return capabilities;
  }

  DataValue DescribeCommands() const override {
    return DataValueFromJson(Json{{"product_type", product_type_},
                                  {"commands", Json::array({Json{{"name", "echo"}},
                                                            Json{{"name", "fail"}}})},
                                  {"streams", Json::array({"state", "diagnostics", "tactile"})}});
  }

  DataValue SnapshotState() override {
    return DataValueFromJson(Json{{"joint_positions", Json::array({1, 2, 3})}});
  }

  DataValue SnapshotDiagnostics() override {
    return DataValueFromJson(Json{{"temperature_reports", Json::array({30, 31, 32})}});
  }

  DataValue SnapshotTactile() override {
    return DataValueFromJson(
        Json{{"sensors", Json::array({Json{{"sensor_id", 1}, {"data", Json::array({7, 8})}}})}});
  }

  DataValue RunCommand(const std::string& name, const DataValue& params) override {
    if (name == "echo") {
      return DataValueFromJson(Json{{"echo", DataValueToJson(params)}});
    }
    if (name == "fail") {
      throw ApplicationError(MakeProblem(502, "sdk_failure", "Bad Gateway", "fake SDK failure"));
    }
    throw ApplicationError(MakeProblem(
        400, "unsupported_command", "Bad Request", "Unsupported command: " + name));
  }

 private:
  std::string product_type_;
};

class FakeHandDeviceFactory final : public IHandDeviceFactory {
 public:
  std::vector<ProductInfo> ListProducts() const override {
    return {ProductInfo{"fake_hand", "Fake Hand", 3, true}};
  }

  std::vector<std::string> ListConnectionTypes() const override {
    return {"fake", "bad_connection"};
  }

  DataValue ConnectionConfigs() const override {
    return DataValueFromJson(Json{{"fake", Json::array({Json{{"serial", "string"}}})}});
  }

  DataValue CommandsForProduct(const std::string& product_type) const override {
    if (product_type != "fake_hand") {
      throw ApplicationError(MakeProblem(
          400, "unsupported_product", "Bad Request", "Unsupported product_type: " + product_type));
    }
    return DataValueFromJson(Json{{"product_type", product_type},
                                  {"commands", Json::array({Json{{"name", "echo"}}})},
                                  {"streams", Json::array({"state", "diagnostics", "tactile"})}});
  }

  std::unique_ptr<IHandDevice> Create(const HandCreateSpec& spec) override {
    ++create_count;
    if (spec.product_type != "fake_hand") {
      throw ApplicationError(MakeProblem(
          400, "unsupported_product", "Bad Request", "Unsupported product_type: " + spec.product_type));
    }
    if (spec.connection.type != "fake") {
      throw ApplicationError(MakeProblem(
          400, "unsupported_connection", "Bad Request",
          "Unsupported connection type: " + spec.connection.type));
    }
    return std::unique_ptr<IHandDevice>(new FakeHandDevice(spec.product_type));
  }

  int create_count = 0;
};

DataValue CreateRequest(const std::string& serial = "A") {
  return DataValueFromJson(
      Json{{"product_type", "fake_hand"},
           {"hand_side", "left"},
           {"connection", Json{{"type", "fake"},
                               {"config", Json{{"serial", serial}, {"hand_device_id", 1}}}}}});
}

std::shared_ptr<FakeHandDeviceFactory> MakeFactory() {
  return std::make_shared<FakeHandDeviceFactory>();
}

Json ToJson(const DataValue& value) {
  return DataValueToJson(value);
}

}  // namespace

TEST(HandServiceTest, CreateListDescribeDeleteHand) {
  auto factory = MakeFactory();
  HandService service(factory);

  Json created = ToJson(service.CreateHand(CreateRequest()));
  ASSERT_EQ(created.value("hand_id", 0), 1);
  EXPECT_EQ(created.value("product_type", ""), "fake_hand");
  EXPECT_EQ(created.value("hand_type", ""), "fake_hand");
  EXPECT_EQ(created.value("hand_side", ""), "left");
  EXPECT_EQ(created.value("conn_method", ""), "fake");

  Json hands = ToJson(service.ListHands());
  ASSERT_TRUE(hands.is_array());
  ASSERT_EQ(hands.size(), 1u);

  Json described = ToJson(service.DescribeHand(1));
  EXPECT_EQ(described.value("hand_id", 0), 1);

  Json removed = ToJson(service.DeleteHand(1));
  EXPECT_TRUE(removed.value("removed", false));
  EXPECT_TRUE(ToJson(service.ListHands()).empty());
}

TEST(HandServiceTest, DuplicateConnectionReturnsExistingSession) {
  auto factory = MakeFactory();
  HandService service(factory);

  Json first = ToJson(service.CreateHand(CreateRequest("same")));
  Json second = ToJson(service.CreateHand(CreateRequest("same")));

  EXPECT_EQ(first.value("hand_id", 0), second.value("hand_id", -1));
  EXPECT_TRUE(second.value("already_exists", false));
  EXPECT_EQ(factory->create_count, 1);
}

TEST(HandServiceTest, CommandDispatchSuccessAndFailure) {
  auto factory = MakeFactory();
  HandService service(factory);
  service.CreateHand(CreateRequest());

  Json result = ToJson(service.RunCommand(1, "echo", DataValueFromJson(Json{{"value", 42}})));
  EXPECT_EQ(result["echo"].value("value", 0), 42);

  try {
    (void)service.RunCommand(1, "missing", DataValue::EmptyObject());
    FAIL() << "unsupported command should throw";
  } catch (const ApplicationError& exc) {
    EXPECT_EQ(exc.problem().status, 400);
    EXPECT_EQ(exc.problem().code, "unsupported_command");
  }

  try {
    (void)service.RunCommand(1, "fail", DataValue::EmptyObject());
    FAIL() << "SDK failure should throw";
  } catch (const ApplicationError& exc) {
    EXPECT_EQ(exc.problem().status, 502);
  }
}

TEST(HandServiceTest, StreamSnapshots) {
  auto factory = MakeFactory();
  HandService service(factory);
  service.CreateHand(CreateRequest());

  EXPECT_TRUE(ToJson(service.GetState(1)).contains("joint_positions"));
  EXPECT_TRUE(ToJson(service.GetDiagnostics(1)).contains("temperature_reports"));
  EXPECT_TRUE(ToJson(service.GetTactile(1)).contains("sensors"));
  EXPECT_TRUE(ToJson(service.ReadStream(1, "state")).contains("joint_positions"));

  try {
    (void)service.ReadStream(1, "unknown");
    FAIL() << "unsupported stream should throw";
  } catch (const ApplicationError& exc) {
    EXPECT_EQ(exc.problem().status, 400);
    EXPECT_EQ(exc.problem().code, "unsupported_stream");
  }
}

TEST(HandServiceTest, UnsupportedProductConnectionAndUnknownHand) {
  auto factory = MakeFactory();
  HandService service(factory);

  Json bad_product = ToJson(CreateRequest());
  bad_product["product_type"] = "bad";
  EXPECT_THROW((void)service.CreateHand(DataValueFromJson(bad_product)), ApplicationError);

  Json bad_connection = ToJson(CreateRequest());
  bad_connection["connection"]["type"] = "bad_connection";
  EXPECT_THROW((void)service.CreateHand(DataValueFromJson(bad_connection)), ApplicationError);

  try {
    (void)service.DescribeHand(999);
    FAIL() << "unknown hand should throw";
  } catch (const ApplicationError& exc) {
    EXPECT_EQ(exc.problem().status, 404);
    EXPECT_EQ(exc.problem().code, "unknown_hand");
  }
}

TEST(HandServiceTest, LegacyCreateShapeStillParses) {
  auto factory = MakeFactory();
  HandService service(factory);

  Json created = ToJson(service.CreateHand(DataValueFromJson(
      Json{{"hand_type", "fake_hand"},
           {"conn_method", "fake"},
           {"conn_config", Json{{"serial", "legacy"}}}})));
  EXPECT_EQ(created.value("hand_id", 0), 1);
  EXPECT_EQ(created.value("product_type", ""), "fake_hand");
}
