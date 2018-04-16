#pragma once

#include <vector>
#include <algorithm>

class Observable;

class Observer {
public:
	virtual ~Observer() = default;
	virtual void update(Observable* obs, void* arg) = 0;
};

class Observable {
public:
	Observable() = default;

	void addObserver(Observer* o) {
		if (o == nullptr) {
			return;
		}
		if (std::find(obs_.begin(), obs_.end(), o) == obs_.end()) {
			obs_.push_back(o);
		}
	}

	void deleteObserver(Observer* o) {
		obs_.erase(std::remove(obs_.begin(), obs_.end(), o), obs_.end());
	}

	void deleteObservers() {
		obs_.clear();
	}

	void notifyObservers() {
		notifyObservers(nullptr);
	}

	void notifyObservers(void* arg) {
		for (auto ob : obs_) {
			ob->update(this, arg);
		}
	}

	bool hasChanged() const {
		return changed_;
	}

	int countObservers() const {
		return obs_.size();
	}

protected:
	void setChanged() {
		changed_ = true;
	}

	void clearChanged() {
		changed_ = false;
	}

private:
	bool changed_ = false;
	std::vector<Observer*> obs_;
};
