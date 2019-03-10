#include "lua_class_map_data.hpp"
#include "../../data/types/type_music.hpp"
#include "../../lua_env/enums/enums.hpp"
#include "../../map.hpp"
#include "../../position.hpp"

namespace elona
{
namespace lua
{

void LuaMapData::bind(sol::state& lua)
{
    auto LuaMapData = lua.create_simple_usertype<MapData>();
    LuaMapData.set("new", sol::no_constructor);

    /**
     * @luadoc field atlas_number num
     *
     * [RW] The map tile atlas to use for this map.
     */
    LuaMapData.set("atlas_number", &MapData::atlas_number);
    /**
     * @luadoc field next_regenerate_date num
     *
     * [RW] The date in hours when this map is next regenerated.
     */
    LuaMapData.set("next_regenerate_date", &MapData::next_regenerate_date);
    /**
     * @luadoc field turn_cost num
     *
     * [RW] The relative time it takes to pass a single turn in this map.
     */
    LuaMapData.set("turn_cost", &MapData::turn_cost);

    /**
     * @luadoc field max_crowd_density num
     *
     * [RW] The maximum crowd density of this map.
     */
    LuaMapData.set("max_crowd_density", &MapData::max_crowd_density);

    /**
     * @luadoc field current_dungeon_level num
     *
     * [R] The current dungeon level of this map.
     */
    LuaMapData.set(
        "current_dungeon_level", sol::readonly(&MapData::max_crowd_density));

    /**
     * @luadoc field tileset num
     *
     * [RW] The tileset type to use for this map.
     */
    LuaMapData.set("tileset", &MapData::tileset);

    /**
     * @luadoc field next_restock_date num
     *
     * [RW] The date in hours when the map is next restocked.
     */
    LuaMapData.set("next_restock_date", &MapData::next_restock_date);

    /**
     * @luadoc field max_item_count num
     *
     * [RW] The maximum number of items this map can hold.
     */
    LuaMapData.set("max_item_count", &MapData::max_item_count);

    /**
     * @luadoc field regenerate_count num
     *
     * [R] The number of times this map has been regenerated.
     */
    LuaMapData.set(
        "regenerate_count", sol::readonly(&MapData::regenerate_count));

    /**
     * @luadoc field designated_spawns bool
     *
     * [RW] If true, reset characters to their initial position on map refresh.
     */
    LuaMapData.set(
        "designated_spawns",
        sol::property(
            [](MapData& d) { return d.designated_spawns == 1; },
            [](MapData& d, bool flag) { d.designated_spawns = flag ? 1 : 0; }));

    /**
     * @luadoc field is_indoors bool
     *
     * [RW] Indicates if this map is indoors.
     */
    LuaMapData.set(
        "is_indoors",
        sol::property(
            [](MapData& d) { return d.indoors_flag == 1; },
            [](MapData& d, bool flag) { d.indoors_flag = flag ? 1 : 2; }));

    /**
     * @luadoc field is_user_map bool
     *
     * [RW] Indicates if this map is a user map.
     */
    LuaMapData.set(
        "is_user_map",
        sol::property(
            [](MapData& d) { return d.user_map_flag == 1; },
            [](MapData& d, bool flag) { d.user_map_flag = flag ? 1 : 0; }));

    /**
     * @luadoc field play_campfire_sound bool
     *
     * [RW] If true, play the campfire ambience.
     */
    LuaMapData.set(
        "play_campfire_sound",
        sol::property(
            [](MapData& d) { return d.play_campfire_sound == 1; },
            [](MapData& d, bool flag) {
                d.play_campfire_sound = flag ? 1 : 0;
            }));

    /**
     * @luadoc field should_regenerate bool
     *
     * [RW] If true, regenerate this map when it is refreshed. This will restock
     * shop inventories.
     */
    LuaMapData.set(
        "should_regenerate",
        sol::property(
            [](MapData& d) { return d.should_regenerate == 0; },
            [](MapData& d, bool flag) { d.should_regenerate = flag ? 0 : 1; }));


    /**
     * @luadoc field is_temporary bool
     *
     * [RW] If true, delete this map when it is exited.
     */
    LuaMapData.set(
        "is_temporary",
        sol::property(
            [](MapData& d) { return d.refresh_type == 0; },
            [](MapData& d, bool flag) { d.refresh_type = flag ? 0 : 1; }));

    /**
     * @luadoc type field MapType
     *
     * [RW] The type of the map.
     */
    LuaMapData.set(
        "type",
        sol::property(
            [](MapData& d) {
                return LuaEnums::MapTypeTable.convert_to_string(
                    static_cast<mdata_t::MapType>(d.type));
            },
            [](MapData& d, const std::string& type) {
                d.type = static_cast<int>(
                    LuaEnums::MapTypeTable.ensure_from_string(type));
            }));

    /**
     * @luadoc stair_up_pos field LuaPosition
     *
     * [R] The position of the map's up stairs.
     */
    LuaMapData.set(
        "stair_up_pos", sol::property([](MapData& d) {
            return Position{d.stair_up_pos % 1000, d.stair_up_pos / 1000};
        }));

    /**
     * @luadoc stair_down_pos field LuaPosition
     *
     * [R] The position of the map's down stairs.
     */
    LuaMapData.set(
        "stair_down_pos", sol::property([](MapData& d) {
            return Position{d.stair_down_pos % 1000, d.stair_down_pos / 1000};
        }));

    /**
     * @luadoc bgm field core.music
     *
     * [RW] The music which will play on entering the map.
     */
    LuaMapData.set("bgm", LUA_API_DATA_PROPERTY(MapData, bgm, the_music_db));

    lua.set_usertype("LuaMapData", LuaMapData);
}

} // namespace lua
} // namespace elona
