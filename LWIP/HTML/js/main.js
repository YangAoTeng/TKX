const { createApp } = Vue

const app = createApp({
    data() {
        return {
            brightness: 50,
            selectedColor: '#ff0000',
            statusMessage: '',
            isError: false,
            sensorData: null,
            eventSource: null
        }
    },
    methods: {
        showStatus(message, isError = false) {
            this.statusMessage = message;
            this.isError = isError;
            setTimeout(() => {
                this.statusMessage = '';
                this.isError = false;
            }, 3000);
        },
        
        updateBrightness() {
            const value = Math.round(this.brightness * 2.55); // 转换为0-255范围
            fetch(`/brightness?value=${value}`)
                .then(response => {
                    if(!response.ok) throw new Error('设置亮度失败');
                    this.showStatus('亮度已更新');
                })
                .catch(error => this.showStatus(error.message, true));
        },
        
        setColor() {
            const color = this.selectedColor;
            const r = parseInt(color.substr(1,2), 16);
            const g = parseInt(color.substr(3,2), 16);
            const b = parseInt(color.substr(5,2), 16);
            
            fetch(`/color?r=${r}&g=${g}&b=${b}`)
                .then(response => {
                    if(!response.ok) throw new Error('设置颜色失败');
                    this.showStatus('颜色已更新');
                })
                .catch(error => this.showStatus(error.message, true));
        },
        
        startRainbow() {
            fetch('/rainbow')
                .then(response => {
                    if(!response.ok) throw new Error('启动彩虹模式失败');
                    this.showStatus('彩虹模式已启动');
                })
                .catch(error => this.showStatus(error.message, true));
        },
        
        setRandom() {
            fetch('/random')
                .then(response => {
                    if(!response.ok) throw new Error('设置随机颜色失败');
                    this.showStatus('随机颜色模式已启动');
                })
                .catch(error => this.showStatus(error.message, true));
        },
        
        initEventSource() {
            this.eventSource = new EventSource('/events');
            
            this.eventSource.addEventListener('sensor_data', (e) => {
                this.sensorData = JSON.parse(e.data);
            });
            
            this.eventSource.onerror = (e) => {
                console.error('SSE连接错误:', e);
                this.eventSource.close();
                this.showStatus('数据连接已断开', true);
            };
        }
    },
    mounted() {
        // 组件挂载后初始化SSE连接
        this.initEventSource();
    },
    beforeUnmount() {
        // 组件销毁前关闭SSE连接
        if(this.eventSource) {
            this.eventSource.close();
        }
    }
})

app.mount('#app')