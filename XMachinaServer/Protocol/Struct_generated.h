// automatically generated by the FlatBuffers compiler, do not modify


#ifndef FLATBUFFERS_GENERATED_STRUCT_FBPROTOCOL_H_
#define FLATBUFFERS_GENERATED_STRUCT_FBPROTOCOL_H_

#include "flatbuffers/flatbuffers.h"

// Ensure the included flatbuffers.h is the same version as when this file was
// generated, otherwise it may not be compatible.
static_assert(FLATBUFFERS_VERSION_MAJOR == 24 &&
              FLATBUFFERS_VERSION_MINOR == 3 &&
              FLATBUFFERS_VERSION_REVISION == 7,
             "Non-compatible flatbuffers version included");

#include "Enum_generated.h"
#include "Transform_generated.h"

namespace FBProtocol {

struct Player;
struct PlayerBuilder;

struct Phero;
struct PheroBuilder;

struct Monster;
struct MonsterBuilder;

struct Player FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef PlayerBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_NAME = 6,
    VT_HP = 8,
    VT_TRANS = 10,
    VT_SPINE_LOOK = 12
  };
  uint32_t id() const {
    return GetField<uint32_t>(VT_ID, 0);
  }
  const ::flatbuffers::String *name() const {
    return GetPointer<const ::flatbuffers::String *>(VT_NAME);
  }
  float hp() const {
    return GetField<float>(VT_HP, 0.0f);
  }
  const FBProtocol::Transform *trans() const {
    return GetPointer<const FBProtocol::Transform *>(VT_TRANS);
  }
  const FBProtocol::Vector3 *spine_look() const {
    return GetPointer<const FBProtocol::Vector3 *>(VT_SPINE_LOOK);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_ID, 4) &&
           VerifyOffset(verifier, VT_NAME) &&
           verifier.VerifyString(name()) &&
           VerifyField<float>(verifier, VT_HP, 4) &&
           VerifyOffset(verifier, VT_TRANS) &&
           verifier.VerifyTable(trans()) &&
           VerifyOffset(verifier, VT_SPINE_LOOK) &&
           verifier.VerifyTable(spine_look()) &&
           verifier.EndTable();
  }
};

struct PlayerBuilder {
  typedef Player Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_id(uint32_t id) {
    fbb_.AddElement<uint32_t>(Player::VT_ID, id, 0);
  }
  void add_name(::flatbuffers::Offset<::flatbuffers::String> name) {
    fbb_.AddOffset(Player::VT_NAME, name);
  }
  void add_hp(float hp) {
    fbb_.AddElement<float>(Player::VT_HP, hp, 0.0f);
  }
  void add_trans(::flatbuffers::Offset<FBProtocol::Transform> trans) {
    fbb_.AddOffset(Player::VT_TRANS, trans);
  }
  void add_spine_look(::flatbuffers::Offset<FBProtocol::Vector3> spine_look) {
    fbb_.AddOffset(Player::VT_SPINE_LOOK, spine_look);
  }
  explicit PlayerBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Player> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Player>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Player> CreatePlayer(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t id = 0,
    ::flatbuffers::Offset<::flatbuffers::String> name = 0,
    float hp = 0.0f,
    ::flatbuffers::Offset<FBProtocol::Transform> trans = 0,
    ::flatbuffers::Offset<FBProtocol::Vector3> spine_look = 0) {
  PlayerBuilder builder_(_fbb);
  builder_.add_spine_look(spine_look);
  builder_.add_trans(trans);
  builder_.add_hp(hp);
  builder_.add_name(name);
  builder_.add_id(id);
  return builder_.Finish();
}

inline ::flatbuffers::Offset<Player> CreatePlayerDirect(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t id = 0,
    const char *name = nullptr,
    float hp = 0.0f,
    ::flatbuffers::Offset<FBProtocol::Transform> trans = 0,
    ::flatbuffers::Offset<FBProtocol::Vector3> spine_look = 0) {
  auto name__ = name ? _fbb.CreateString(name) : 0;
  return FBProtocol::CreatePlayer(
      _fbb,
      id,
      name__,
      hp,
      trans,
      spine_look);
}

struct Phero FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef PheroBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_LEVEL = 6,
    VT_OFFSET_DIST = 8
  };
  uint32_t id() const {
    return GetField<uint32_t>(VT_ID, 0);
  }
  uint8_t level() const {
    return GetField<uint8_t>(VT_LEVEL, 0);
  }
  const FBProtocol::Position_Vec2 *offset_dist() const {
    return GetPointer<const FBProtocol::Position_Vec2 *>(VT_OFFSET_DIST);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_ID, 4) &&
           VerifyField<uint8_t>(verifier, VT_LEVEL, 1) &&
           VerifyOffset(verifier, VT_OFFSET_DIST) &&
           verifier.VerifyTable(offset_dist()) &&
           verifier.EndTable();
  }
};

struct PheroBuilder {
  typedef Phero Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_id(uint32_t id) {
    fbb_.AddElement<uint32_t>(Phero::VT_ID, id, 0);
  }
  void add_level(uint8_t level) {
    fbb_.AddElement<uint8_t>(Phero::VT_LEVEL, level, 0);
  }
  void add_offset_dist(::flatbuffers::Offset<FBProtocol::Position_Vec2> offset_dist) {
    fbb_.AddOffset(Phero::VT_OFFSET_DIST, offset_dist);
  }
  explicit PheroBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Phero> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Phero>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Phero> CreatePhero(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t id = 0,
    uint8_t level = 0,
    ::flatbuffers::Offset<FBProtocol::Position_Vec2> offset_dist = 0) {
  PheroBuilder builder_(_fbb);
  builder_.add_offset_dist(offset_dist);
  builder_.add_id(id);
  builder_.add_level(level);
  return builder_.Finish();
}

struct Monster FLATBUFFERS_FINAL_CLASS : private ::flatbuffers::Table {
  typedef MonsterBuilder Builder;
  enum FlatBuffersVTableOffset FLATBUFFERS_VTABLE_UNDERLYING_TYPE {
    VT_ID = 4,
    VT_TYPE = 6,
    VT_BT_TYPE = 8,
    VT_POS_2 = 10,
    VT_ROT_Y = 12,
    VT_TARGET_PLAYER_ID = 14,
    VT_TARGET_MONSTER_ID = 16
  };
  uint32_t id() const {
    return GetField<uint32_t>(VT_ID, 0);
  }
  FBProtocol::MONSTER_TYPE type() const {
    return static_cast<FBProtocol::MONSTER_TYPE>(GetField<uint8_t>(VT_TYPE, 0));
  }
  FBProtocol::MONSTER_BT_TYPE bt_type() const {
    return static_cast<FBProtocol::MONSTER_BT_TYPE>(GetField<uint8_t>(VT_BT_TYPE, 0));
  }
  const FBProtocol::Position_Vec2 *pos_2() const {
    return GetPointer<const FBProtocol::Position_Vec2 *>(VT_POS_2);
  }
  float rot_y() const {
    return GetField<float>(VT_ROT_Y, 0.0f);
  }
  int32_t target_player_id() const {
    return GetField<int32_t>(VT_TARGET_PLAYER_ID, 0);
  }
  int32_t target_monster_id() const {
    return GetField<int32_t>(VT_TARGET_MONSTER_ID, 0);
  }
  bool Verify(::flatbuffers::Verifier &verifier) const {
    return VerifyTableStart(verifier) &&
           VerifyField<uint32_t>(verifier, VT_ID, 4) &&
           VerifyField<uint8_t>(verifier, VT_TYPE, 1) &&
           VerifyField<uint8_t>(verifier, VT_BT_TYPE, 1) &&
           VerifyOffset(verifier, VT_POS_2) &&
           verifier.VerifyTable(pos_2()) &&
           VerifyField<float>(verifier, VT_ROT_Y, 4) &&
           VerifyField<int32_t>(verifier, VT_TARGET_PLAYER_ID, 4) &&
           VerifyField<int32_t>(verifier, VT_TARGET_MONSTER_ID, 4) &&
           verifier.EndTable();
  }
};

struct MonsterBuilder {
  typedef Monster Table;
  ::flatbuffers::FlatBufferBuilder &fbb_;
  ::flatbuffers::uoffset_t start_;
  void add_id(uint32_t id) {
    fbb_.AddElement<uint32_t>(Monster::VT_ID, id, 0);
  }
  void add_type(FBProtocol::MONSTER_TYPE type) {
    fbb_.AddElement<uint8_t>(Monster::VT_TYPE, static_cast<uint8_t>(type), 0);
  }
  void add_bt_type(FBProtocol::MONSTER_BT_TYPE bt_type) {
    fbb_.AddElement<uint8_t>(Monster::VT_BT_TYPE, static_cast<uint8_t>(bt_type), 0);
  }
  void add_pos_2(::flatbuffers::Offset<FBProtocol::Position_Vec2> pos_2) {
    fbb_.AddOffset(Monster::VT_POS_2, pos_2);
  }
  void add_rot_y(float rot_y) {
    fbb_.AddElement<float>(Monster::VT_ROT_Y, rot_y, 0.0f);
  }
  void add_target_player_id(int32_t target_player_id) {
    fbb_.AddElement<int32_t>(Monster::VT_TARGET_PLAYER_ID, target_player_id, 0);
  }
  void add_target_monster_id(int32_t target_monster_id) {
    fbb_.AddElement<int32_t>(Monster::VT_TARGET_MONSTER_ID, target_monster_id, 0);
  }
  explicit MonsterBuilder(::flatbuffers::FlatBufferBuilder &_fbb)
        : fbb_(_fbb) {
    start_ = fbb_.StartTable();
  }
  ::flatbuffers::Offset<Monster> Finish() {
    const auto end = fbb_.EndTable(start_);
    auto o = ::flatbuffers::Offset<Monster>(end);
    return o;
  }
};

inline ::flatbuffers::Offset<Monster> CreateMonster(
    ::flatbuffers::FlatBufferBuilder &_fbb,
    uint32_t id = 0,
    FBProtocol::MONSTER_TYPE type = FBProtocol::MONSTER_TYPE_NONE,
    FBProtocol::MONSTER_BT_TYPE bt_type = FBProtocol::MONSTER_BT_TYPE_NONE,
    ::flatbuffers::Offset<FBProtocol::Position_Vec2> pos_2 = 0,
    float rot_y = 0.0f,
    int32_t target_player_id = 0,
    int32_t target_monster_id = 0) {
  MonsterBuilder builder_(_fbb);
  builder_.add_target_monster_id(target_monster_id);
  builder_.add_target_player_id(target_player_id);
  builder_.add_rot_y(rot_y);
  builder_.add_pos_2(pos_2);
  builder_.add_id(id);
  builder_.add_bt_type(bt_type);
  builder_.add_type(type);
  return builder_.Finish();
}

}  // namespace FBProtocol

#endif  // FLATBUFFERS_GENERATED_STRUCT_FBPROTOCOL_H_
