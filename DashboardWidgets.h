#include <initializer_list>
#include <tuple>
#include <string>
#include <vector>

#include "SmartDashboard/SmartDashboard.h"
#include "SmartDashboard/SendableChooser.h"

#include "Misc.h"

using std::initializer_list;
using std::tuple;
using std::string;
using std::vector;

template <typename T_value> class RadioList;

template <typename T_value>
class RadioListOption {
private:
    friend class RadioList<T_value>;
    string name;
    T_value value;
    bool is_default;
public:
    RadioListOption(string name, T_value value)
        :name(name), value(value), is_default(false)
    {}
    RadioListOption(string name, T_value value, bool is_default)
        :name(name), value(value), is_default(is_default)
    {}
};

template <typename T_value>
class RadioList {
private:
    SendableChooser *chooser;
    vector<RadioListOption<T_value>*> options;
    T_value selected;
    string selected_name;
public:
    RadioList()
        :selected(), selected_name()
    {
        chooser = new SendableChooser();
    }

    RadioList (const initializer_list<RadioListOption<T_value> > &values)
        :RadioList()
    {
        for (auto it = values.begin(); it != values.end(); ++it) {
            AddOption(it->name, it->value, it->is_default);
        }
    }

    void SendToDashboard (string keyname) {
        SmartDashboard::PutData(keyname, chooser);
    }

    void AddOption (string name, T_value value, bool is_default = false) {
        auto opt = new RadioListOption<T_value>(name, value);
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

    ~RadioList() {
        for (auto it = options.begin(); it != options.end(); ++it) {
            delete *it;
        }
    }

private:
    void UpdateSelected() {
        auto opt = (RadioListOption<T_value>*)chooser->GetSelected();
        if (opt) {
            selected = opt->value;
            selected_name = opt->name;
        }
    }
};
