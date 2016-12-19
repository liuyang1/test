defmodule Parallel do
  def map(collection, fun) do
    parent = self()

    processes = Enum.map(collection, fn(e) ->
        spawn_link(fn() ->
            send(parent, {self(), fun.(e)})
            end)
        end)

    Enum.map(processes, fn(pid) ->
        receive do
          {^pid, result} -> result
        end
      end)
  end
end

slow_double = fn(x) -> :timer.sleep(1000); x * 2 end
:timer.tc(fn() -> Enum.map([1, 2, 3, 4], slow_double) end)
:timer.tc(fn() -> Parallel.map([1, 2, 3, 4], slow_double) end)
