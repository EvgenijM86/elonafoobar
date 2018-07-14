#pragma once
#include "spec.hpp"
#include "snail/application.hpp"
#include "thirdparty/microhcl/hcl.hpp"

#include <iostream>

namespace elona
{

/***
 * Contains a layout of valid config options, to be used by the config
 * class for validating values set on it.
 */
class config_def : public spec::object
{
public:
    enum class option_platform
    {
        all,
        desktop,
        android,
    };

    struct metadata
    {
        // True if the config section/option is visible in the options menu.
        bool visible = true;

        // True if the config option is loaded before application is
        // initialized (screen size, language...)
        bool preload = false;

        // True if runtime enum variants should cause errors on
        // missing translations. Set to false when enum variants are
        // human-readable themselves.
        bool translate_variants = true;

        // Platform this option applies to. On other platforms it will
        // be hidden.
        option_platform platform = option_platform::all;

        // Boolean config options that need to be the specified value
        // for this option to be visible in the config menu.
        // Non-boolean options are ignored.
        std::map<std::string, bool> dependencies;

        bool is_visible() const
        {
            switch(platform)
            {
            case option_platform::desktop:
                return visible && !snail::application::is_android();
            case option_platform::android:
                return visible && snail::application::is_android();
            case option_platform::all:
            default:
                return visible;
            }

        }
    };

    config_def() : spec::object("config")
    {
        locale_root = "core.locale.config.menu";
    }

    ~config_def() = default;

    void clear()
    {
        spec::object::clear();
        data.clear();
    }

    const metadata& get_metadata(const spec_key& key) const { return data.at(key); }

    const std::string& get_locale_root() const { return locale_root; }

    void post_visit(const spec_key&, const spec::section_def&);
    void pre_visit_section(const spec_key&, const hcl::Object&);
    void pre_visit_item(const spec_key&, const hcl::Object&);
    void pre_visit_bare_value(const spec_key&, const hcl::Value&);

private:
    /***
    * Gets the visibility of a config item considering the visibility
    * of its parent sections. This is needed for determining whether
    * or not to save certain items based on visibility.
    */
    bool is_child_visible(const spec_key&);

    std::string locale_root;
    std::map<std::string, metadata> data;
};
}
