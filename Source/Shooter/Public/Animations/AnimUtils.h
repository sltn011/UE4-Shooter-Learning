#pragma once

class UAnimSequenceBase;

class AnimUtils
{
public:

	template<typename T>
	static T *FindNotifyByClass(
		UAnimSequenceBase *Animation
	)
	{
		if (!Animation) {
			return nullptr;
		}

		TArray<FAnimNotifyEvent> &NotifyEvents = Animation->Notifies;
		for (FAnimNotifyEvent &NotifyEvent : NotifyEvents) {
			T *FoundNotify = Cast<T>(NotifyEvent.Notify);
			if (FoundNotify) {
				return FoundNotify;
			}
		}
		return nullptr;
	}

};