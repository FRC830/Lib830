#include <initializer_list>
#include <string>
#include <vector>

#include "SmartDashboard/SendableChooser.h"

namespace Lib830 {
namespace Dashboard {

using std::string;
using std::vector;

template <typename T_value> class Select;

template <typename T_value>
class SelectOption {
private:
    friend class Select<T_value>;
    string name;
    T_value value;
    bool is_default;
public:
    SelectOption(string name, T_value value)
        :name(name), value(value), is_default(false)
    {}
    SelectOption(string name, T_value value, bool is_default)
        :name(name), value(value), is_default(is_default)
    {}
};

template <typename T_value>
class Select : public Widget {
private:
    SendableChooser *chooser;
    vector<SelectOption<T_value>*> options;
    T_value selected;
    string selected_name;
public:
    Select()
        :selected(), selected_name()
    {
        chooser = new SendableChooser();
    }

    Select (const std::initializer_list<SelectOption<T_value> > &values)
        :Select()
    {
        for (auto it = values.begin(); it != values.end(); ++it) {
            AddOption(it->name, it->value, it->is_default);
        }
    }

    virtual void sendToDashboard (string keyname) {
        SmartDashboard::PutData(keyname, chooser);
    }

    void AddOption (string name, T_value value, bool is_default = false) {
        auto opt = new SelectOption<T_value>(name, value);
        options.push_back(opt);
        if (is_default) {
            chooser->AddDefault(name, opt);
            selected_name = name;
            selected = value;
        }
        else {
            chooser->AddObject(name, opt);
        }
    }

    T_value GetSelected() {
        UpdateSelected();
        return selected;
    }

    string GetSelectedName() {
        UpdateSelected();
        return selected_name;
    }

    ~Select() {
        for (auto it = options.begin(); it != options.end(); ++it) {
            delete *it;
        }
    }

private:
    void UpdateSelected() {
        auto opt = (SelectOption<T_value>*)chooser->GetSelected();
        if (opt) {
            selected = opt->value;
            selected_name = opt->name;
        }
    }
};

}}
