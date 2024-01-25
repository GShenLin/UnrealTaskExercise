// Fill out your copyright notice in the Description page of Project Settings.


#include "TaskActor.h"

#include "Tasks/Task.h"

using namespace UE::Tasks;

void CalcFunc()
{
	UE_LOG(LogTemp, Log, TEXT("CalcFunc Begin"));
	for (int i = 0; i < 9000000; i++)
	{
		FString s(TEXT("CalcFunc"));
		s = s + s;
	}
	UE_LOG(LogTemp, Log, TEXT("CalcFunc Completed"));
}

ATaskActor::ATaskActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATaskActor::BeginPlay()
{
	Super::BeginPlay();

	// ?囹芰眎瓷舝
	/*TaskLaunch();
	TaskWait();
	TaskBusyWait();
	TaskGetRusult();
	TaskPrerequisites();
	TaskNested();
	TaskPipe();
	TaskEvent();*/
}

void ATaskActor::TaskLaunch()
{
	// 使用Lambda函数启动一个任务
	// 测试过程中 不会卡死 是开启新线程进行
	UE_LOG(LogTemp, Log, TEXT("Task Begin Launch"));
	FTask TaskA =  Launch(TEXT("Task Launch"), []
		{
			UE_LOG(LogTemp, Log, TEXT("CalcFunc Begin"));
			for (int i = 0; i < 9000000; i++)
			{
				FString s(TEXT("CalcFunc"));
				s = s + s;
			}
			UE_LOG(LogTemp, Log, TEXT("CalcFunc Completed"));
			
		}
	);
	
	UE_LOG(LogTemp, Log, TEXT("Task End"));
}

void ATaskActor::TaskWait()
{
	// 发起任务 等待任务结束 使用函数的方式发起任务 
	FTask Task = Launch(TEXT("Task Wait"), &CalcFunc);

	UE_LOG(LogTemp, Log, TEXT("Task Wait Begin"));
	// 会阻断主线程
	Task.Wait();
	UE_LOG(LogTemp, Log, TEXT("Task Wait End"));
}

void ATaskActor::TaskBusyWait()
{
	// 首先启动任务 A
	FTask TaskA = Launch(TEXT("TaskA Busy Wait"), []
		{
			FPlatformProcess::Sleep(1.0f);
			UE_LOG(LogTemp, Log, TEXT("TaskA End"));
		}
	);

	UE_LOG(LogTemp, Log, TEXT("Task Busy Wait Begin"));

	// 任务 B 在任务 A 之前完成
	FTask TaskB = Launch(TEXT("TaskB Busy Wait"), []
		{
			FPlatformProcess::Sleep(0.5f);
			UE_LOG(LogTemp, Log, TEXT("TaskB End"));
		}
	);

	// 如果您 BusyWait 任务 A，它将等待该过程完成。
	// 同时，任务 B 仍将被处理，因此任务 B 将首先完成，我们将继续
	// 会阻断主线程
	TaskA.BusyWait();

	UE_LOG(LogTemp, Log, TEXT("Task Busy Wait End"));
}

void ATaskActor::TaskGetResult()
{
	// 在任务中返回布尔值
	TTask<bool> BoolTask = Launch(TEXT("Task Return Bool"), []
		{
			return true;
		}
	);

	// 检查 GetRusult 上的内部内容
	bool bResult = BoolTask.GetResult();
	UE_LOG(LogTemp, Log, TEXT("BoolTask Result = %s"), bResult ? TEXT("true") : TEXT("false"));
}

void ATaskActor::TaskPrerequisites()
{
	// 启动任务A
	FTask TaskA = Launch(TEXT("Task Prereqs TaskA"), []
		{
			FPlatformProcess::Sleep(1.0f);
			UE_LOG(LogTemp, Log, TEXT("TaskA End"));
		}
	);
	
	// 任务 B 和任务 C 在任务 A 完成之前不会启动
	FTask TaskB = Launch(TEXT("Task Prereqs TaskB"), []
		{
			FPlatformProcess::Sleep(2.2f);
			UE_LOG(LogTemp, Log, TEXT("TaskB End"));
		}, TaskA
	);
	FTask TaskC = Launch(TEXT("Task Prereqs TaskC"), []
		{
			FPlatformProcess::Sleep(0.5f);
			UE_LOG(LogTemp, Log, TEXT("TaskC End"));
		}, TaskA
	);

	// 在任务 B 和任务 C 完成之前，任务 D 不会启动
	FTask TaskD = Launch(TEXT("Task Prereqs TaskD"), []
		{
			FPlatformProcess::Sleep(1.f);
			UE_LOG(LogTemp, Log, TEXT("TaskD End"));
		}, Prerequisites(TaskB, TaskC)
	);

	// wait会阻断主游戏线程
	//TaskD.Wait();
	UE_LOG(LogTemp, Log, TEXT("Task Prerequisites End"));
}

void ATaskActor::TaskNested()
{
	// 启动父任务 A 后，在任务 A 中启动任务 B，并阻止直到两个任务完成。
	FTask TaskA = Launch(TEXT("Task Nasted Outer"), []
		{
			FTask TaskB = Launch(TEXT("Task Nasted Inner"), &CalcFunc);
			AddNested(TaskB);
		}
	);

	//TaskA.Wait();
	UE_LOG(LogTemp, Log, TEXT("Task Nasted End"));
}

void ATaskActor::TaskPipe()
{
	//FPipe Pipe{ TEXT("Pipe") };  如果直接析构会崩溃 因为TaskA还未完成  所以pipe的定义放在别的地方 

	// 在管道中启动任务 A
	FTask TaskA = Pipe.Launch(TEXT("Task Pipe TaskA"), []
		{
			FPlatformProcess::Sleep(1.0f);
			UE_LOG(LogTemp, Log, TEXT("TaskA End"));
		}
	);

	// 管道启动任务 B，但在任务 A 完成之前不会启动
	FTask TaskB = Pipe.Launch(TEXT("Task Pipe TaskB"), []
		{
			UE_LOG(LogTemp, Log, TEXT("TaskB End"));
		}
	);

	// 等到任务 B 完成，任务 A 和 B 按顺序完成。
	//TaskB.Wait();
	UE_LOG(LogTemp, Log, TEXT("Task Pipe End"));
}

void ATaskActor::TaskEvent()
{
	FTaskEvent Event{ TEXT("Event") };

	// 在 Launch 中将 TaskEvent 传递到参数的末尾
	Launch(TEXT("Task Event"), []
		{
			UE_LOG(LogTemp, Log, TEXT("TaskEvent Completed"));
		}, Event
	);

	// 触发并执行注册为事件的任务
	Event.Trigger();
}
