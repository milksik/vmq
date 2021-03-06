// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <sstream>
#include <chrono>
#include <ctime>
#include <list>

#define LOCK_QUEUE	1
#define MPSC_QUEUE	2

#define QUEUE_TYPE	MPSC_QUEUE



#include "queue/mpsc-queue.hpp"

#include "actor.h"
#include "player.h"
#include "player_worker.h"
#include "player_task_manager.h"
// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.



