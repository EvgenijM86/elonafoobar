local Chara = Elona.require("Chara")
local GUI = Elona.require("GUI")
local I18N = Elona.require("I18N")
local Internal = Elona.require("Internal")
local Item = Elona.require("Item")

return {
   name = "rilian",
   root = "core.locale.talk.unique.rilian",
   nodes = {
      __start = function()
         local flag = Internal.get_quest_flag("puppys_cave")
         if flag == 1000 then
            return "quest_completed"
         elseif flag == 0 then
            return "quest_ask"
         elseif flag == 1 then
            return "quest_check"
         end

         return "__IGNORED__"
      end,
      quest_completed = {
         text = {
            {"complete"},
         },
      },
      quest_ask = {
         text = {
            {"quest.dialog"},
         },
         choices = {
            {"quest_yes", "quest.choices.do_it"},
            {"quest_no", "__BYE__"}
         },
      },
      quest_yes = {
         text = {
            GUI.show_journal_update_message,
            {"quest.do_it"},
         },
         on_finish = function()
            Internal.set_quest_flag("puppys_cave", 1)
         end
      },
      quest_no = {
         text = {
            {"quest.bye"},
         },
      },
      quest_check = function()
         if Chara.find("core.poppy", "Allies") == nil then
            return "quest_waiting"
         end

         return "quest_finish"
      end,
      quest_waiting = {
         text = {
            {"quest.waiting"}
         }
      },
      quest_finish = {
         text = {
            {"quest.end"},
         },
         on_finish = function()
            Item.create(Chara.player().position, "core.cooler_box", 0)
            Item.create(Chara.player().position, "core.gold_piece", 2500)
            Item.create(Chara.player().position, "core.platinum_coin", 2)

            GUI.txt(I18N.get("core.locale.quest.completed"))
            GUI.play_sound("core.complete1")
            GUI.txt(I18N.get("core.locale.common.something_is_put_on_the_ground"))
            GUI.show_journal_update_message()
            Internal.set_quest_flag("puppys_cave", 1000)

            Chara.find("core.poppy", "Allies"):vanquish()
            local poppy = Chara.create(31, 4, "core.poppy")
            poppy.role = 3
         end
      }
   }
}
