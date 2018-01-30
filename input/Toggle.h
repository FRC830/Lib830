/*
 * Toggle.h
 *
 *  Created on: Aug 16, 2017
 *      Author: RatPack
 */

#ifndef LIB830_INPUT_TOGGLE_H_
#define LIB830_INPUT_TOGGLE_H_

class Toggle {
private:
	bool prev_state;
	bool toggle_state;
public:
	Toggle(bool toggle_state = false):prev_state(false), toggle_state(toggle_state){}
	bool toggle (bool button_state) {
		if (button_state && prev_state != button_state) {
			toggle_state = !toggle_state;
		}
		prev_state = button_state;
		return toggle_state;
	}

	void Set(bool state) {
		toggle_state = state;
	}

	operator bool () {
		return toggle_state;
	}

};



#endif /* LIB830_INPUT_TOGGLE_H_ */
