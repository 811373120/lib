#pragma once
template<typename T>
class SingletonBase
{
public:
	static T* Instance() {
		if (NULL != m_instance) {
			return m_instance;
		}
	}


private:
	static T* m_instance;

	TSingleton(const TSingleton&);
	TSingleton& operator=(const TSingleton&);

protected:
	SingletonBase() {}
	virtual ~SingletonBase() {}
};
template <typename T>
T* SingletonBase<T>::m_inst = NULL;
