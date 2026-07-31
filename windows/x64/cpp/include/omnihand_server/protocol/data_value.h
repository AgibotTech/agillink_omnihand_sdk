#ifndef AGILINK_OMNIHAND_SERVER_PROTOCOL_DATA_VALUE_H
#define AGILINK_OMNIHAND_SERVER_PROTOCOL_DATA_VALUE_H

#include <cstdint>
#include <initializer_list>
#include <map>
#include <stdexcept>
#include <string>
#include <utility>
#include <variant>
#include <vector>

namespace agilink {
namespace omnihand {
namespace server {
namespace protocol {

class DataValue {
 public:
  using Bytes = std::vector<std::uint8_t>;
  using Array = std::vector<DataValue>;
  using Object = std::map<std::string, DataValue>;

  enum class Type {
    kNull,
    kBool,
    kInt,
    kDouble,
    kString,
    kBytes,
    kArray,
    kObject,
  };

  DataValue();
  DataValue(std::nullptr_t);
  DataValue(bool value);
  DataValue(int value);
  DataValue(std::int64_t value);
  DataValue(double value);
  DataValue(const char* value);
  DataValue(std::string value);
  DataValue(Bytes value);
  DataValue(Array value);
  DataValue(Object value);

  static DataValue EmptyArray();
  static DataValue EmptyObject();
  static DataValue ArrayOf(std::initializer_list<DataValue> values);
  static DataValue ObjectOf(std::initializer_list<std::pair<std::string, DataValue>> values);

  Type type() const;
  bool IsNull() const;
  bool IsBool() const;
  bool IsInt() const;
  bool IsDouble() const;
  bool IsNumber() const;
  bool IsString() const;
  bool IsBytes() const;
  bool IsArray() const;
  bool IsObject() const;

  bool AsBool() const;
  std::int64_t AsInt() const;
  double AsDouble() const;
  const std::string& AsString() const;
  const Bytes& AsBytes() const;
  const Array& AsArray() const;
  const Object& AsObject() const;

  Array& MutableArray();
  Object& MutableObject();

  bool Contains(const std::string& key) const;
  const DataValue* Find(const std::string& key) const;
  const DataValue& At(const std::string& key) const;
  void Set(std::string key, DataValue value);
  void Erase(const std::string& key);
  std::size_t Size() const;
  bool Empty() const;

  std::string DebugString() const;

 private:
  using Storage =
      std::variant<std::nullptr_t, bool, std::int64_t, double, std::string, Bytes, Array, Object>;

  template <typename T>
  const T& CheckedGet(const char* expected_type) const {
    if (const auto* value = std::get_if<T>(&value_)) {
      return *value;
    }
    throw std::invalid_argument(std::string("DataValue is not ") + expected_type);
  }

  template <typename T>
  T& CheckedGetMutable(const char* expected_type) {
    if (auto* value = std::get_if<T>(&value_)) {
      return *value;
    }
    throw std::invalid_argument(std::string("DataValue is not ") + expected_type);
  }

  Storage value_;
};

}  // namespace protocol
}  // namespace server
}  // namespace omnihand
}  // namespace agilink

#endif  // AGILINK_OMNIHAND_SERVER_PROTOCOL_DATA_VALUE_H
