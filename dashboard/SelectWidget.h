#pragma once

#include <initializer_list>
#include <string>
#include <vector>

#include "Widget.h"
#include "frc/smartdashboard/SendableChooser.h"

namespace Lib830 {

	using std::string;
	using std::vector;

	template <typename T_value> class SelectWidget;

	template <typename T_value>
	class SelectWidgetOption {
	private:
		friend class SelectWidget<T_value>;
		string name;
		T_value value;
		bool is_default;
	public:
		SelectWidgetOption(string name, T_value value)
			:name(name), value(value), is_default(false)
		{}
		SelectWidgetOption(string name, T_value value, bool is_default)
			:name(name), value(value), is_default(is_default)
		{}
	};

	template <typename T_value>
	class SelectWidget : public Widget {
	private:
		frc::SendableChooser<SelectWidgetOption<T_value>> chooser;
		vector<SelectWidgetOption<T_value>*> options;
		T_value selected;
		string selected_name;
	public:
		SelectWidget()
			:selected(), selected_name("")
		{}

		SelectWidget (const std::initializer_list<SelectWidgetOption<T_value> > &values)
			:SelectWidget()
		{
			for (auto it = values.begin(); it != values.end(); ++it) {
				AddOption(it->name, it->value, it->is_default);
			}
		}

		virtual void sendToDashboard (string keyname) {
			frc::SmartDashboard::PutData(keyname, &chooser);
		}

		void AddOption (string name, T_value value, bool is_default = false) {
			auto opt = new SelectWidgetOption<T_value>(name, value);
			options.push_back(opt);
			if (is_default) {
				chooser.AddDefault(name, opt);
				selected_name = name;
				selected = value;
			}
			else {
				chooser.AddObject(name, opt);
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

		~SelectWidget() {
			for (auto it = options.begin(); it != options.end(); ++it) {
				delete *it;
			}
		}

	private:
		void UpdateSelected() {
			auto opt = (SelectWidgetOption<T_value>*)chooser.GetSelected();
			if (opt) {
				selected = opt->value;
				selected_name = opt->name;
			}
		}
	};

}
