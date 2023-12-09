## Vulkan绘制三角形

在Vulkan当中的画一个三角形流程可以分为如下:

- 创建一个 VkInstance
- 选择支持的硬件设备（VkPhysicalDevice）
- 创建用于Draw和Presentation的VkDevice 和 VkQueue
- 创建窗口(window)、窗口表面(window surface)和交换链 (Swap Chain)
- 将Swap Chain Image 包装到 VkImageView
- 创建一个指定Render Target和用途的RenderPass
- 为RenderPass创建FrameBuffer
- 设置PipeLine
- 为每个可能的Swap Chain Image分配并记录带有绘制命令的Command Buffer
- 通过从Swap Chain获取图像在上面绘制，提交正确的Commander Buffer，并将绘制完的图像返回到Swap Chain去显示。

### **Semaphores**

**Semaphores用于在GPU队列操作之间增添顺序**。队列操作是指我们提交给队列的工作，可以是在Command Buffer中，也可以是在函数中，我们稍后会看到。在我们这里Queue分别是Graphics Queue和Presentation Queue。Semaphores用于对同一队列内和不同队列之间的工作进行控制。

Semaphore要么是无信号的，要么是有信号的。它以没有信号的方式开始。

我们使用semaphore对队列操作进行排序的方式是在一个队列操作中提供发出Semaphore相同的信号量，在另一个队列操作中提供与等待semaphore相同的信号量。

例子代码:

```C++
VkCommandBuffer A, B = ... // record command buffers
VkSemaphore S = ... // create a semaphore

// enqueue A, signal S when done - starts executing immediately
vkQueueSubmit(work: A, signal: S, wait: None)

// enqueue B, wait on S to start
vkQueueSubmit(work: B, signal: None, wait: S)
```

请注意，在此代码片段中，对 vkQueueSubmit() 的两个调用都会立即返回 - **等待仅发生在 GPU 上**。 CPU 继续运行而不会阻塞。为了让 CPU 等待，我们需要一个不同的同步原语，我们现在将对其进行描述

### **Fence**

Fence也有类似的作用，它被用来同步执行，但它是为了**在CPU侧上控制执行顺序**。简单地说，如果主机需要知道GPU什么时候完成了什么，我们就使用Fence。

与Semaphore类似，Fence也是处于有信号或无信号的状态。每当我们提交要执行的工作时，我们可以为该工作附加一个Fence。当工作完成后，Fence将被发出信号。然后，我们可以让主机等待Fence发出信号，保证在主机继续工作之前工作已经完成。

一个具体的例子是拍摄屏幕截图。假设我们已经在GPU上完成了必要的工作。现在需要把图像从GPU上传到主机上，然后把内存保存到文件中。我们有执行传输的Command Buffer A和Fence F。我们用 Fence F提交Command Buffer A，然后立即告诉主机等待F的信号。这导致主机阻塞，直到Command Buffer A执行完毕。因此，我们可以安全地让主机将文件保存到磁盘，因为内存传输已经完成。

上面说的的伪代码如下所示

```cpp
VkCommandBuffer A = ... // record command buffer with the transfer
VkFence F = ... // create the fence

// enqueue A, start work immediately, signal F when done
vkQueueSubmit(work: A, fence: F)

vkWaitForFence(F) // blocks execution until A has finished executing

save_screenshot_to_disk() // can't run until the transfer has finished
```

与Semaphore的例子不同，这个例子会阻止主机的执行。这意味着主机除了等待执行完毕外不会做任何事情。在这个例子中，我们必须确保在将屏幕截图保存到磁盘之前，传输已经完成。

Fence必须被手动重置，以使其恢复到无信号的状态。这是因为Fence是用来控制主机的执行的，所以主机可以决定何时重置Fence。与此相对应的是Semaphores，它是用来命令GPU上的工作而不需要主机参与。

综上所述，Semaphores用于指定GPU上操作的执行顺序，而Fence则用于保持CPU和GPU之间的同步性。

Swap Chain操作和等待前一帧完成。我们希望将Semaphore用于Swap Chain操作，因为它们发生在 GPU 上，因此如果我们可以提供帮助，我们不想让主机等待。为了等待前一帧完成，我们想使用Fence，因为我们需要主机等待。这样我们就不会一次绘制超过一帧。