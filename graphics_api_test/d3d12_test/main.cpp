
#include <CGUtils/api.hpp>
#include <CGUtils/misc.hpp>
#include <CGUtils/parallel.hpp>
#include <coroutine>
#include <atomic>

using namespace wzz::misc;
class async_manual_reset_event : no_copy_t, no_move_t
{
public:
	async_manual_reset_event(bool initiallySet = false) noexcept
	: m_state(initiallySet ? this : nullptr)
	{

	}

	bool is_set() const noexcept
	{
		return m_state.load(std::memory_order_acquire) == this;
	}
	void set() noexcept;
	void reset() noexcept
	{
		void* old_value = this;
		m_state.compare_exchange_strong(old_value, nullptr, std::memory_order_acquire);
	}

	struct awaiter;
	awaiter operator co_await() const noexcept;

private:
	friend struct awaiter;

	mutable std::atomic<void*> m_state;
};

struct async_manual_reset_event::awaiter
{
	awaiter(const async_manual_reset_event& event) noexcept
	:m_event(event)
	{

	}

	~awaiter()
	{
		// std::cout << "awaiter desctruct" << std::endl;
	}

	bool await_ready() const noexcept
	{
		return m_event.is_set();
	}
	bool await_suspend(std::coroutine_handle<> awaitingCoroutine) noexcept
	{
		const void* const set_state = &m_event;

		m_awaitingCoroutine = awaitingCoroutine;

		void* old_value = m_event.m_state.load(std::memory_order_acquire);
		do
		{
			if(old_value == set_state) return false;
			
			m_next = static_cast<awaiter*>(old_value);

		}while(!m_event.m_state.compare_exchange_weak(old_value, this, std::memory_order_release, std::memory_order_acquire));

		return true;
	}
	void await_resume() noexcept
	{
		std::cout << "await_resume is called" << std::endl;
	}

private:
	friend struct async_manual_reset_event;

	const async_manual_reset_event& m_event;
	std::coroutine_handle<> m_awaitingCoroutine;
	awaiter* m_next = nullptr;
};

void async_manual_reset_event::set() noexcept
{
	void* old_value = m_state.exchange(this, std::memory_order_acq_rel);
	if(old_value != this)
	{
		auto* waiters = static_cast<awaiter*>(old_value);
		std::vector<std::jthread> consume_tasks;
		while(waiters)
		{
			auto next = waiters->m_next;
			consume_tasks.emplace_back([waiters]{waiters->m_awaitingCoroutine.resume();});
			waiters = next;
		}
	}
}

async_manual_reset_event::awaiter async_manual_reset_event::operator co_await() const noexcept
{
	return awaiter{*this};
}

struct task
{
	struct my_promise
	{
		std::suspend_never initial_suspend() { return {}; }
		std::suspend_never final_suspend() noexcept { return {}; }
		void unhandled_exception() {}
		task get_return_object() { return {}; }
		void return_void() {}
	};

	using promise_type = my_promise;
};

task start_async_task(async_manual_reset_event& event, int thread_index, int x)
{
	std::cout << "Thread " << thread_index << " start, value is " << x << std::endl;
	co_await event;
	// std::vector<int> arr((x + 1) * 1000000, 2);
	// for(int i = 0; i < arr.size(); i++)
	// {
	// 	arr[i] = (arr[arr.size() - i - 1] + arr[i]) / 2;
	// }
	std::cout << "Thread " << thread_index << " finished, value is " << x << std::endl;
}


int main(int argc, char** argv){
	using namespace wzz::thread;
	
	async_manual_reset_event event;

	parallel_forrange(0, 10, [&](int thread_index, int x){
		start_async_task(event, thread_index, x);
	}, 10);
	
	std::cout << "after parall_forrange" << std::endl;

	event.set();

	std::cout << "after event set" << std::endl;

	using namespace wzz::d3d12;

	//Device dev;
	// draw resources
	// 1. vertex buffer/ index buffer
	// 2. pso, shader
	// 3. cbv/srv/uav, descriptor heap
	// 4. heap for resources
	


	
}