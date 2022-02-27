#include "audio.h"

#include "opengl/gl/gl.h"

#include "main.h"
#include "res.h"
#include "font.h"
#include "gltools.h"
#include "shell.h"
#include "soundtouch/SoundTouch.h"

bool AudioUtils::init = false;

ALCdevice* AudioUtils::alDev;
ALCcontext* AudioUtils::alCtx;

AudioUtils::Complex::Complex(_Complex float comp) : comp(comp) {}
AudioUtils::Complex::~Complex(){}

float AudioUtils::Complex::MagnitudeSqr(){
    return real * real + imag * imag;
}

float AudioUtils::Complex::Magnitude(){
    return __builtin_sqrt(real * real + imag * imag);
}

bool AudioUtils::CheckALError(const char* tag, const char* file, int line){
    int loopCnt = 0;
	for (ALenum error = alGetError(); loopCnt < 32 && error != AL_NO_ERROR; error = alGetError(), ++loopCnt){
		const char* pMsg;
		switch (error){
		case AL_INVALID_NAME: pMsg = "Invalid name"; break;
		case AL_INVALID_ENUM: pMsg = "Invalid enum"; break;
		case AL_INVALID_VALUE: pMsg = "Invalid value"; break;
		case AL_INVALID_OPERATION: pMsg = "Invalid operation"; break;
		case AL_OUT_OF_MEMORY: pMsg = "Out of memory"; break;
		default: pMsg = "Unknown Error";
		}
		DebugError("[OpenAL Error] %s %s(0x%x) at %s:%d\n", tag, pMsg, error, file, line);
	}
	return loopCnt != 0;
}

void AudioUtils::FFT(_Complex float* input, int sizebit, bool inv){
    int size = 1 << sizebit;
    int* rev = new int[size];
    rev[0] = 0;
    for (int i = 1; i < size; i++){
        rev[i] = (rev[i >> 1] >> 1) | ((i & 1) ? (1 << (sizebit - 1)) : 0);
    }
    for (int i = 0; i < size; i++){
        if (i < rev[i]){
            _Complex float tmp;
            tmp = input[i];
            input[i] = input[rev[i]];
            input[rev[i]] = tmp;
        }
    }
    delete[] rev;
    for (int i = 1; i <= sizebit; i++){
        int half = (1 << (i - 1)), num = (1 << i);
        for (int j = 0; j < (1 << (sizebit - i)); j++){
            _Complex float o, m;
            m = 1.0f + 0.0if;
            o = __builtin_sin(PI / num) * (inv ? -1.0if : 1.0if) + __builtin_cos(PI / num);
            for (int k = (j << i); k < (j << i) + half; k++){
                _Complex float A = input[k], B = input[k + half];
                input[k] = A + B * o;//单位根份数
                input[k + half] = A - B * o;
                m *= o;
            }
        }
    }
    if (!inv){
        for (int i = 0; i < size; i++){
            input[i] /= size;
        }
    }
}

void AudioUtils::InitOpenAL(){
    if (init){
        return;
    }

    init = true;

    alDev = alcOpenDevice(NULL);
    alCtx = alcCreateContext(alDev, NULL);

    alcMakeContextCurrent(alCtx);

    PrintOpenALInfo();
    LoadOpenALPreset();
}

void AudioUtils::UninitOpenAL(){
    if (init){
        init = false;
        
        alcCloseDevice(alDev);
        alcDestroyContext(alCtx);
    }
}

void AudioUtils::PrintOpenALInfo(){
    DebugLog("OpenAL Enabled");
    DebugLog("OpenAL Version %s", alGetString(AL_VERSION));
    DebugLog("OpenAL Renderer %s", alGetString(AL_RENDERER));
    DebugLog("OpenAL Vendor %s", alGetString(AL_VENDOR));
    //DebugLog("OpenAL Extensions %s", alGetString(AL_EXTENSIONS));
}

void AudioUtils::LoadOpenALPreset(){
    alListener3f(AL_POSITION, 0.0f, 0.0f, 0.0f);
    alListener3f(AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    alListener3f(AL_ORIENTATION, 0.0f, 1.0f, 0.0f);

    alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
}

AudioPlayerWindow::PlayButton::PlayButton(AudioPlayerWindow* window) : window(window) {}
AudioPlayerWindow::PlayButton::~PlayButton(){}

bool AudioPlayerWindow::PlayButton::Trigger(Vector2 pos){
    return pos.x >= -0.5f && pos.x <= 0.5f && pos.y >= -0.9f && pos.y <= -0.3f;
}

void AudioPlayerWindow::PlayButton::Hover(){
    hover = true;
}

void AudioPlayerWindow::PlayButton::Click(Vector2 pos){
    if (window->IsLaunched()){
        window->Stop();
    }else{
        window->Launch();
    }
}

void AudioPlayerWindow::PlayButton::Leave(){
    hover = false;
}

void AudioPlayerWindow::PlayButton::Render(){
    if (hover){
        glColor3f(1.0f, 0.5f, 0.0f);
    }else{
        glColor3f(0.0f, 0.4f, 1.0f);
    }
    GLUtils::DrawRoundRect(-0.5f, -0.9f, 1.0f, 0.6f, 0.2f, 0.05f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_TRIANGLES);
    glVertex2f(-0.15f, -0.8f);
    glVertex2f(-0.15f, -0.4f);
    glVertex2f(0.25f, -0.6f);
    glEnd();
}

AudioPlayerWindow::ProgressBar::ProgressBar(AudioPlayerWindow* window) : window(window) {}
AudioPlayerWindow::ProgressBar::~ProgressBar(){}

bool AudioPlayerWindow::ProgressBar::Trigger(Vector2 pos){
    return pos.x >= this->pos - 0.05f && pos.x <= this->pos + 0.05f && pos.y >= -0.2f && pos.y <= 0.0f;
}

void AudioPlayerWindow::ProgressBar::Click(Vector2 pos){
    origin = this->pos;
}

void AudioPlayerWindow::ProgressBar::Drag(Vector2 dir){
    ALint offset;

    pos = Clamp(origin + dir.x, -0.6f, 0.6f);
    offset = Clamp((int)(((pos + 0.6f) / 1.2f) * window->alAudioSize), 0, window->alAudioSize);

    alSourcei(window->alSrc, AL_SAMPLE_OFFSET, offset);
    window->alAudioOffset = offset;
}

void AudioPlayerWindow::ProgressBar::Hover(){
    hover = true;
}

void AudioPlayerWindow::ProgressBar::Leave(){
    hover = false;
}

void AudioPlayerWindow::ProgressBar::Render(){
    if (window->loaded){
        if (window->launched)
            alGetSourcei(window->alSrc, AL_SAMPLE_OFFSET, &window->alAudioOffset);

        glLineWidth(10.0f);
        glColor3f(0.6f, 0.6f, 0.6f);
        glBegin(GL_LINES);
        glVertex2f(-0.6f, -0.1f);
        glVertex2f(0.6f, -0.1f);
        glEnd();
        glLineWidth(1.0f);

        pos = Lerp(-0.6f, 0.6f, (float)window->alAudioOffset / window->alAudioSize);
        if (hover){
            glColor3f(0.0f, 0.0f, 0.3f);
        }else{
            glColor3f(0.0f, 0.0f, 0.5f);
        }
        GLUtils::DrawRect(pos - 0.05f, -0.2f, pos + 0.05f, 0.0f);
    }
}

AudioPlayerWindow::LoopOption::LoopOption(AudioPlayerWindow* window) : window(window) {}
AudioPlayerWindow::LoopOption::~LoopOption(){}

bool AudioPlayerWindow::LoopOption::Trigger(Vector2 pos){
    return pos.x >= 0.7f && pos.x <= 0.9f && pos.y >= -0.2f && pos.y <= 0.0f;
}

void AudioPlayerWindow::LoopOption::Click(Vector2 pos){
    loop = window->IsLoop();
    loop = !loop;
    window->SetLoop(loop);
    DebugLog("AudioPlayerWindow::LoopOption State %s", loop ? "Looping" : "Default");
}

void AudioPlayerWindow::LoopOption::Render(){
    loop = window->IsLoop();
    if (loop){
        glColor3f(0.0f, 0.0f, 1.0f);
    }else{
        glColor3f(0.0f, 0.0f, 0.0f);
    }
    GLUtils::DrawRect(0.7f, -0.2f, 0.9f, 0.0f);
}

AudioPlayerWindow::LoopItem::LoopItem(AudioPlayerWindow* window) : window(window) {}
AudioPlayerWindow::LoopItem::~LoopItem(){}

const wchar_t* AudioPlayerWindow::LoopItem::GetName(){
    return window->IsLoop() ? L"循环:开" : L"循环:关";
}

void AudioPlayerWindow::LoopItem::OnClick(){
    bool loop = window->IsLoop();
    loop = !loop;
    window->SetLoop(loop);
    DebugLog("AudioPlayerWindow::LoopItem State %s", loop ? "Looping" : "Default");
}

AudioPlayerWindow::DisplayModeItem::DisplayModeItem(AudioPlayerWindow* window) : window(window) {}
AudioPlayerWindow::DisplayModeItem::~DisplayModeItem(){}

const wchar_t* AudioPlayerWindow::DisplayModeItem::GetName(){
    return window->displayWave ? L"显示模式:波形" : L"显示模式:频谱";
}

void AudioPlayerWindow::DisplayModeItem::OnClick(){
    window->displayWave = !window->displayWave;
    DebugLog("AudioPlayerWindow::DisplayModeItem State %s", window->displayWave ? "Wave" : "Frequency");
}

AudioPlayerWindow::AudioPlayerWindow(){
    DebugLog("AudioPlayerWindow Launched");

    AudioUtils::InitOpenAL();

    uiMgr = new UIManager();

    uiMgr->AddButton(new PlayButton(this));
    uiMgr->AddButton(new ProgressBar(this));
    uiMgr->AddButton(new LoopOption(this));

    basicMenu = new Menu();
    basicMenu->AddItem(new MenuItem(L"加载", MENUITEM_LAMBDA_TRANS(AudioPlayerWindow)[](AudioPlayerWindow* window){
        window->OnMenuAccel(IDM_LOAD, false);
    }, this));
    basicMenu->AddItem(new LoopItem(this));
    basicMenu->AddItem(new DisplayModeItem(this));
}

AudioPlayerWindow::~AudioPlayerWindow(){
    DebugLog("AudioPlayerWindow Destroyed");
    if (uiMgr) delete uiMgr;
    if (alAudioData) delete[] (char*)alAudioData;

    if (launched){
        alSourceStop(alSrc);

        alDeleteBuffers(1, &alBuf);
        alDeleteSources(1, &alSrc);
    }
}

bool AudioPlayerWindow::IsFocus(){
    return focus;
}

void AudioPlayerWindow::DrawLineGraph(float* height, size_t size){
    glDisable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
    glBegin(GL_LINE_STRIP);
    glColor3f(1.0f, 1.0f, 1.0f);
    for (int i = 0; i < size; i++){
        float rate = i / (float)size;
        glVertex2f(rate * 2.0f - 1.0f, height[i]);
    }
    glEnd();
}

void AudioPlayerWindow::DrawAmplitudeGraph(float* height, size_t size){
    glDisable(GL_LINE_SMOOTH);
    glLineWidth(1.0f);
    glBegin(GL_LINES);
    for (int i = 0; i < size; i++){
        float rate = i / (float)size;
        if (height[i] > 0.0f){
            glColor3f(rate, 1.0f - rate, 0.0f);
            glVertex2f(rate * 2.0f - 1.0f, 0.0f);
            glVertex2f(rate * 2.0f - 1.0f, height[i]);
        }
    }
    glEnd();
}

void AudioPlayerWindow::RenderGraph(){
    const int bit = 12;
    ALint offset = GetOffset();

    _Complex float samples[1 << bit];
    float height[1 << 10];

    if (offset + (1 << bit) >= alAudioSize){
        return;
    }

    if (alSampleSize == 4 && alChannels == 2){
        for (int i = 0; i < (1 << bit); i++){
            samples[i] = ((short*)alAudioData)[(i + offset) << 1];
        }
    }else if (alSampleSize == 2 && alChannels == 1){
        for (int i = 0; i < (1 << bit); i++){
            samples[i] = ((short*)alAudioData)[i + offset];
        }
    }else if (alSampleSize == 2 && alChannels == 2){
        for (int i = 0; i < (1 << bit); i++){
            samples[i] = ((short)(((char*)alAudioData)[(i + offset) << 1] - 0x80) << 8);
        }
    }else if (alSampleSize == 1 && alChannels == 1){
        for (int i = 0; i < (1 << bit); i++){
            samples[i] = ((short)(((char*)alAudioData)[i + offset] - 0x80) << 8);
        }
    }

    if (displayWave){
        for (int i = 0; i < (1 << 10); i++){
            height[i] = AudioUtils::Complex(samples[i << (bit - 10)]).real * 0.0000152587890625f + 0.5f;
        }
        DrawLineGraph(height, 1 << 10);
    }else{
        AudioUtils::FFT(samples, bit, false);
        for (int i = 0; i < (1 << 10); i++){
            height[i] = __builtin_log(AudioUtils::Complex(samples[i << (bit - 10)]).MagnitudeSqr()) * 0.1f;
        }
        DrawAmplitudeGraph(height, 1 << 10);
    }
}

void AudioPlayerWindow::OnRender(){
    glClearColor(0.3f, 0.3f, 0.3f, 0.0f);

    glClear(GL_COLOR_BUFFER_BIT);

    glEnable(GL_BLEND);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_LIGHTING);
    glEnable(GL_ALPHA_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, 0.0, 2.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    if (path.GetLength() == 0){
        glColor3f(1.0f, 0.5f, 0.0f);
        glRasterPos2f(-1.0f, 1.0f - 30.0f / size.y);
        glDrawCNString(L"尚未存在音频文件");
        glRasterPos2f(-1.0f, 1.0f - 60.0f / size.y);
        glDrawCNString(L"可以将文件拖拽至此处");
        return;
    }

    RenderGraph();

    uiMgr->Render();

    if (IsLaunched()){
        //TODO 不知道为什么，设置无效
        Vector3 pos = Main::data->audioPos;
        alListener3f(AL_POSITION, pos.x, pos.y, pos.z);
    }
}

void AudioPlayerWindow::OnClose(){}

void AudioPlayerWindow::OnTimer(int id){
    if (IsLaunched()){
        Main::RequestRender();
    }
}

void AudioPlayerWindow::OnChar(char c){}

void AudioPlayerWindow::OnUnichar(wchar_t c){}

void AudioPlayerWindow::OnResize(int x, int y){
    if (x == 0 || y == 0){
        return;
    }
    size.x = x;
    size.y = y;
}

void AudioPlayerWindow::OnMouseMove(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
}

void AudioPlayerWindow::OnLeftDown(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
    uiMgr->LeftDown();
}

void AudioPlayerWindow::OnLeftUp(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
    uiMgr->LeftUp();
}

void AudioPlayerWindow::OnRightDown(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
    Main::SetMenu(basicMenu);
}

void AudioPlayerWindow::OnRightUp(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
}

void AudioPlayerWindow::OnMouseHover(int key, int x, int y){}

void AudioPlayerWindow::OnMouseLeave(){}

void AudioPlayerWindow::OnFocus(){
    focus = true;
}

void AudioPlayerWindow::OnKillFocus(){
    focus = false;
}

void AudioPlayerWindow::OnMouseWheel(int delta){}

void AudioPlayerWindow::OnMenuAccel(int id, bool accel){
    DebugLog("AudioPlayerWindow::OnMenuAccel %d %s", id, accel ? "true" : "false");
    switch (id){
    case IDM_LOAD:
        OnInsLoad();
        break;
    }
}

void AudioPlayerWindow::OnDropFileA(const char* path){}

void AudioPlayerWindow::OnDropFileW(const wchar_t* path){
    DebugLog("AudioPlayerWindow::OnDropFileW %S", path);
    PreloadFile(WString(path));
}

void AudioPlayerWindow::OnInsLoad(){
    // 暂不使用 L"PCM音频文件(*.wav)\0*.wav\0所有音频类型(.*)\0*.*\0"，此状态下发现Shell时可能的环境错误
    // 若要使用请拖入文件
    WString file = ShellFileSelectWindow(WString(IDS_WAVFILE_FILTER), OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_EXPLORER);
    if (file.GetLength() == 0){
        DebugLog("Stop Loading");
        return;
    }
    PreloadFile(file);
}

void AudioPlayerWindow::PreloadFile(WString file){
    if (!file.EndsWith(L".wav")){
        WString message(IDS_WAVFILE_FORM_WARNING);
        WString caption(IDS_WAVFILE_FORM_WARNING_CAPTION);
        switch (ShellMsgBox(caption, message)){
        case MSGBOX_NO:
            DebugLog("AudioPlayerWindow::PreloadFile Stop Load File");
            return;
        case MSGBOX_YES:
            DebugLog("AudioPlayerWindow::PreloadFile Preparing FFmpeg");
            if (!ShellFFmpeg(file, L".\\temp.wav")){
                DebugError("AudioPlayerWindow::PreloadFile ShellFFmpegW Failed");
                return;
            }
            path = file;
            LoadFile(L".\\temp.wav");
            File(L".\\temp.wav").Delete();
            return;
        case MSGBOX_CANCEL:
            path = file;
            break;
        }
    }else{
        path = file;
    }
    LoadFile(path);
}

void AudioPlayerWindow::LoadFile(WString file){
    if (file.GetLength() == 0){
        return;
    }

    if (loaded){
        DebugLog("AudioPlayerWindow::LoadFile Unload %S", path.GetString());
        if (launched){
            Stop();
        }
        alDeleteBuffers(1, &alBuf);
        alDeleteSources(1, &alSrc);
        loaded = false;
    }

    DebugLog("AudioPlayerWindow::LoadFile From %S", file.GetString());

    HANDLE hFile = CreateFileW(
        file.GetString(),
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE){
        DebugError("AudioPlayerWindow::LoadFile File Open Failed");
        return;
    }

    DebugLog("OpenAL Started");

    alGenSources(1, &alSrc);
    alGenBuffers(1, &alBuf);

    alSourcef(alSrc, AL_REFERENCE_DISTANCE, 1.0f);
    alSourcef(alSrc, AL_ROLLOFF_FACTOR, 2.0f);
    alSourcef(alSrc, AL_MAX_DISTANCE, 100.0f);

    // analyse data segment
    DWORD specNum;
    DWORD readLen;
    DWORD dataLen;
    WAVEFORMATEX wav;
    bool fmtRead = false;
    ReadFile(hFile, &specNum, 4, NULL, NULL);
    if (specNum != *(DWORD*)"RIFF"){
        CloseHandle(hFile);
        DebugError("AudioPlayerWindow::LoadFile File Magic Number 'RIFF' Not Found");
        return;
    }
    ReadFile(hFile, &specNum, 4, NULL, NULL);
    ReadFile(hFile, &specNum, 4, NULL, NULL);
    if (specNum != *(DWORD*)"WAVE"){
        CloseHandle(hFile);
        DebugError("AudioPlayerWindow::LoadFile File Magic Number 'WAVE' Not Found");
        return;
    }
    ReadFile(hFile, &specNum, 4, &readLen, NULL);
    while (true){
        if (readLen != 4){
            CloseHandle(hFile);
            return;
        }
        if (specNum == *(DWORD*)"fmt "){
            ReadFile(hFile, &specNum, 4, &readLen, NULL);
            if (specNum != 0x10){
                CloseHandle(hFile);
                DebugError("AudioPlayerWindow::LoadFile File Format Length Is Not 0x10 [%d]", specNum);
                return;
            }
            ReadFile(hFile, &wav, 0x10, NULL, NULL);
            wav.wBitsPerSample = wav.nBlockAlign << 3;
            fmtRead = true;
            ReadFile(hFile, &specNum, 4, &readLen, NULL);
            continue;
        }else if (specNum == *(DWORD*)"data"){
            ReadFile(hFile, &dataLen, 4, NULL, NULL);
            break;
        }
        ReadFile(hFile, &specNum, 4, NULL, NULL);
        SetFilePointer(hFile, specNum, NULL, FILE_CURRENT);
        ReadFile(hFile, &specNum, 4, &readLen, NULL);
    }
    if (!fmtRead){
        CloseHandle(hFile);
        DebugError("AudioPlayerWindow::LoadFile File Format Not Found");
        return;
    }
    ALint format = GetWaveFormat(&wav);
    if (format == -1){
        CloseHandle(hFile);
        DebugError("AudioPlayerWindow::LoadFile File Format Unrecognized");
        return;
    }

    char* fileData = new char[dataLen];
    ReadFile(hFile, fileData, dataLen, NULL, NULL);
    alBufferData(alBuf, format, fileData, dataLen, wav.nSamplesPerSec);// 11025 44100
    CloseHandle(hFile);

    alAudioData = fileData;
    alAudioSize = dataLen / wav.nBlockAlign;
    alAudioLen = dataLen / wav.nAvgBytesPerSec;
    alSampleSize = wav.nBlockAlign;
    alChannels = wav.nChannels;
    alAudioOffset = 0;

    DebugLog("AudioPlayerWindow::LoadFile Success");

    loaded = true;
}

bool AudioPlayerWindow::IsLoaded(){
    return loaded;
}

void AudioPlayerWindow::Launch(){
    if (path.GetLength() == 0)
        return;

    if (!loaded){
        DebugError("AudioPlayerWindow::Launch Is Not Loaded");
        return;
    }

    ALint cnt;

    alGetSourcei(alSrc, AL_BUFFERS_QUEUED, &cnt);
    if (cnt == 0){
        alSourceQueueBuffers(alSrc, 1, &alBuf);
    }
    
    alSourcePlay(alSrc);
    alSourcei(alSrc, AL_SAMPLE_OFFSET, alAudioOffset);

    DebugLog("AudioPlayerWindow::Launch Success");

    launched = true;
}

void AudioPlayerWindow::Stop(){
    alGetSourcei(alSrc, AL_SAMPLE_OFFSET, &alAudioOffset);
    alSourceStop(alSrc);
    alSourceUnqueueBuffers(alSrc, 1, &alBuf);

    launched = false;
}

bool AudioPlayerWindow::IsLaunched(){
    ALint state;

    alGetSourcei(alSrc, AL_SOURCE_STATE, &state);
    if (state == AL_PLAYING){
        launched = true;
    }else{
        launched = false;
    }
    return launched;
}

bool AudioPlayerWindow::IsLoop(){
    ALint loop;
    alGetSourcei(alSrc, AL_LOOPING, &loop);
    return loop == AL_TRUE;
}

void AudioPlayerWindow::SetLoop(bool loop){
    alSourcei(alSrc, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

ALint AudioPlayerWindow::GetOffset(){
    if (!loaded)
        return 0;
    if (launched)
        alGetSourcei(alSrc, AL_SAMPLE_OFFSET, &alAudioOffset);
    return alAudioOffset;
}

ALint AudioPlayerWindow::GetWaveFormat(PWAVEFORMATEX lpwav){
    ALenum format = AL_NONE;
    switch (lpwav->nChannels){
    case 1:
        switch (lpwav->nBlockAlign){
        case 1:
            format = AL_FORMAT_MONO8;
            break;
        case 2:
            format = AL_FORMAT_MONO16;
            break;
        }
        break;
    case 2:
        switch (lpwav->nBlockAlign){
        case 2:
            format = AL_FORMAT_STEREO8;
            break;
        case 4:
            format = AL_FORMAT_STEREO16;
            break;
        }
        break;
    }
    return format;
}

AudioCaptureWindow::ProgressBar::ProgressBar(AudioCaptureWindow* window) : window(window) {}
AudioCaptureWindow::ProgressBar::~ProgressBar(){}

bool AudioCaptureWindow::ProgressBar::Trigger(Vector2 pos){
    return pos.x >= 0.8f && pos.x <= 1.0f && pos.y >= this->pos - 0.05f && pos.y <= this->pos + 0.05f;
}

void AudioCaptureWindow::ProgressBar::Click(Vector2 pos){
    origin = this->pos;
}

void AudioCaptureWindow::ProgressBar::Drag(Vector2 dir){
    pos = Clamp(origin + dir.y, -0.8f, 0.8f);
    window->soundTouch->setPitchSemiTones(pos * 1.25f * 12.0f);
}

void AudioCaptureWindow::ProgressBar::Hover(){
    hover = true;
}

void AudioCaptureWindow::ProgressBar::Leave(){
    hover = false;
}

void AudioCaptureWindow::ProgressBar::Render(){
    glLineWidth(10.0f);
    glColor3f(0.6f, 0.6f, 0.6f);
    glBegin(GL_LINES);
    glVertex2f(0.9f, -0.8f);
    glVertex2f(0.9f, 0.8f);
    glEnd();
    glLineWidth(1.0f);

    if (hover){
        glColor3f(0.0f, 0.0f, 0.3f);
    }else{
        glColor3f(0.0f, 0.0f, 0.5f);
    }
    GLUtils::DrawRect(0.8f, pos - 0.05f, 1.0f, pos + 0.05f);
}

AudioCaptureWindow::CaptureItem::CaptureItem(AudioCaptureWindow* window) : window(window) {}
AudioCaptureWindow::CaptureItem::~CaptureItem(){}

const wchar_t* AudioCaptureWindow::CaptureItem::GetName(){
    return window->capture ? L"录音:开" : L"录音:关";
}

void AudioCaptureWindow::CaptureItem::OnClick(){
    window->capture ? window->Stop() : window->Launch();
    DebugLog("AudioCaptureWindow::CaptureItem State %s", window->capture ? "Capturing" : "Stopped");
}

AudioCaptureWindow::DisplayModeItem::DisplayModeItem(AudioCaptureWindow* window) : window(window) {}
AudioCaptureWindow::DisplayModeItem::~DisplayModeItem(){}

const wchar_t* AudioCaptureWindow::DisplayModeItem::GetName(){
    return window->displayWave ? L"显示模式:波形" : L"显示模式:频谱";
}

void AudioCaptureWindow::DisplayModeItem::OnClick(){
    window->displayWave = !window->displayWave;
    DebugLog("AudioCaptureWindow::DisplayModeItem State %s", window->displayWave ? "Wave" : "Frequency");
}

AudioCaptureWindow::AdjushWaveItem::AdjushWaveItem(AudioCaptureWindow* window) : window(window) {}
AudioCaptureWindow::AdjushWaveItem::~AdjushWaveItem(){}

const wchar_t* AudioCaptureWindow::AdjushWaveItem::GetName(){
    return window->adjustWave ? L"调整波形:开" : L"调整波形:关";
}

void AudioCaptureWindow::AdjushWaveItem::OnClick(){
    window->adjustWave = !window->adjustWave;
    DebugLog("AudioCaptureWindow::AdjushWaveItem State %s", window->adjustWave ? "On" : "Off");
}

AudioCaptureWindow::AudioCaptureWindow(){
    DebugLog("AudioCaptureWindow Launched");

    AudioUtils::InitOpenAL();
    
    //SoundTouch 貌似在我的电脑上性能有限，不能很好做到实时变音
    soundTouch = new soundtouch::SoundTouch();
    soundTouch->setChannels(1);
    soundTouch->setSampleRate(freq);
    soundTouch->setPitchSemiTones(0);
    soundTouch->setTempo(1.0f);
    soundTouch->clear();

    uiMgr = new UIManager();

    uiMgr->AddButton(new ProgressBar(this));

    basicMenu = new Menu();
    basicMenu->AddItem(new CaptureItem(this));
    basicMenu->AddItem(new DisplayModeItem(this));
    basicMenu->AddItem(new AdjushWaveItem(this));
}

AudioCaptureWindow::~AudioCaptureWindow(){
    DebugLog("AudioCaptureWindow Destroyed");
    if (uiMgr) delete uiMgr;
    if (capBuf) delete (short*)capBuf;
    if (recBuf) delete (short*)recBuf;
    if (freqBuf) delete freqBuf;

    if (capDev){
        alcCaptureCloseDevice(capDev);
    }

    if (soundTouch){
        soundTouch->clear();
        delete soundTouch;
    }
}

void AudioCaptureWindow::ProcessInput(){
    ALint cnt;
    ALint offset = (capOffset & ((1 << bit) - 1));
    float sum;

    alcGetIntegerv(capDev, ALC_CAPTURE_SAMPLES, 1, &cnt);
    cnt = Min(cnt, (1 << bit) - offset);

    alcCaptureSamples(capDev, (soundtouch::SAMPLETYPE*)capBuf + offset, cnt);

    if (soundTouch->numUnprocessedSamples() <= (1 << (bit + queueBit)) && tail - head < (1 << queueBit) - 1){
        soundTouch->putSamples((soundtouch::SAMPLETYPE*)capBuf + offset, cnt);
    }

    capOffset += cnt;
    
    if (displayWave && adjustWave){
        for (int i = 0; i < (1 << bit); i++){
            short val = ((short*)capBuf)[(i + capOffset) & ((1 << bit) - 1)];
            freqBuf[i] = val;
            sum += Abs((float)val);
        }
        if (sum * ratio < (1ll << (bit + 15)) * 0.01f){
            if (ratio < 100)
                ratio *= 10;
        }else if (sum * ratio > (1ll << (bit + 15))){
            if (ratio > 1)
                ratio /= 10;
        }
    }else{
        for (int i = 0; i < (1 << bit); i++){
            freqBuf[i] = ((short*)capBuf)[(i + capOffset) & ((1 << bit) - 1)];
        }
    }

    AudioUtils::FFT(freqBuf, bit, false);

    if (displayWave){
        glDisable(GL_LINE_SMOOTH);
        glLineWidth(1.0f);
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 1.0f, 1.0f);
        if (adjustWave){
            for (int i = 0; i < 1024; i++){
                float rate = i / 1024.0f;
                float amp = Clamp(((short*)capBuf)[((i << (bit - 10)) + capOffset) & ((1 << bit) - 1)] * 0.000030517578125f * ratio, -1.0f, 1.0f);
                glVertex2f(rate * 2.0f - 1.0f, (amp + 1.0f) * 0.5f);
            }
        }else{
            for (int i = 0; i < 1024; i++){
                float rate = i / 1024.0f;
                float amp = Clamp(((short*)capBuf)[((i << (bit - 10)) + capOffset) & ((1 << bit) - 1)] * 0.000030517578125f, -1.0f, 1.0f);
                glVertex2f(rate * 2.0f - 1.0f, (amp + 1.0f) * 0.5f);
            }
        }
        glEnd();
    }else{
        glDisable(GL_LINE_SMOOTH);
        glLineWidth(1.0f);
        glBegin(GL_LINES);
        for (int i = 0; i < 1024; i++){
            float rate = i / 1024.0f;
            float amp = Clamp(__builtin_log(AudioUtils::Complex(freqBuf[i << (bit - 10)]).MagnitudeSqr()) * 0.1f, 2.0f / size.y, 1.0f);
            glColor3f(rate, 1.0f - rate, 0.0f);
            glVertex2f(rate * 2.0f - 1.0f, (-amp + 1.0f) * 0.5f);
            glVertex2f(rate * 2.0f - 1.0f, (amp + 1.0f) * 0.5f);
        }
        glEnd();
    }
}

void AudioCaptureWindow::ProcessOutput(){
    ALint cnt;
    ALint offset = (recOffset & ((1 << bit) - 1));

    cnt = Min((ALint)soundTouch->numSamples(), (1 << bit) - offset);

    soundTouch->receiveSamples((soundtouch::SAMPLETYPE*)recBuf + offset, cnt);
    if (cnt == (1 << bit) - offset)
        UpdateBuffer(recBuf, 1 << (bit + 1));
    
    recOffset += cnt;

    for (int i = 0; i < (1 << bit); i++)
        freqBuf[i] = ((short*)recBuf)[(i + recOffset) & ((1 << bit) - 1)];
    AudioUtils::FFT(freqBuf, bit, false);

    if (displayWave){
        glDisable(GL_LINE_SMOOTH);
        glLineWidth(1.0f);
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 1.0f, 1.0f);
        if (adjustWave){
            for (int i = 0; i < 1024; i++){
                float rate = i / 1024.0f;
                float amp = Clamp(((short*)recBuf)[((i << (bit - 10)) + recOffset) & ((1 << bit) - 1)] * 0.000030517578125f * ratio, -1.0f, 1.0f);
                glVertex2f(rate * 2.0f - 1.0f, (amp - 1.0f) * 0.5f);
            }
        }else{
            for (int i = 0; i < 1024; i++){
                float rate = i / 1024.0f;
                float amp = Clamp(((short*)recBuf)[((i << (bit - 10)) + recOffset) & ((1 << bit) - 1)] * 0.000030517578125f, -1.0f, 1.0f);
                glVertex2f(rate * 2.0f - 1.0f, (amp - 1.0f) * 0.5f);
            }
        }
        glEnd();
    }else{
        glDisable(GL_LINE_SMOOTH);
        glLineWidth(1.0f);
        glBegin(GL_LINES);
        for (int i = 0; i < 1024; i++){
            float rate = i / 1024.0f;
            float amp = Clamp(__builtin_log(AudioUtils::Complex(freqBuf[i << (bit - 10)]).MagnitudeSqr()) * 0.1f, 2.0f / size.y, 1.0f);
            glColor3f(rate, 1.0f - rate, 0.0f);
            glVertex2f(rate * 2.0f - 1.0f, (-amp - 1.0f) * 0.5f);
            glVertex2f(rate * 2.0f - 1.0f, (amp - 1.0f) * 0.5f);
        }
        glEnd();
    }
}

bool AudioCaptureWindow::IsFocus(){
    return focus;
}

void AudioCaptureWindow::OnRender(){
    ALint cnt;

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINES);
    glVertex2f(-1.0f, 0.0f); glVertex2f(1.0f, 0.0f);
    glEnd();

    if (capture){
        ProcessInput();
        ProcessOutput();
    }

    uiMgr->Render();
}

void AudioCaptureWindow::OnCreate(){
    Launch();
}

void AudioCaptureWindow::OnClose(){}

void AudioCaptureWindow::OnTimer(int id){
    Main::RequestRender();
}

void AudioCaptureWindow::OnChar(char c){}

void AudioCaptureWindow::OnUnichar(wchar_t c){}

void AudioCaptureWindow::OnResize(int x, int y){
    size.x = x;
    size.y = y;
}

void AudioCaptureWindow::OnMouseMove(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
}

void AudioCaptureWindow::OnLeftDown(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
    uiMgr->LeftDown();
}

void AudioCaptureWindow::OnLeftUp(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
    uiMgr->LeftUp();
}

void AudioCaptureWindow::OnRightDown(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
    Main::SetMenu(basicMenu);
}

void AudioCaptureWindow::OnRightUp(int x, int y){
    cursorPos.x = 2.0f * x / size.x - 1.0f;
    cursorPos.y = 2.0f * y / size.y - 1.0f;
    uiMgr->CursorMove(cursorPos);
}

void AudioCaptureWindow::OnMouseHover(int key, int x, int y){}

void AudioCaptureWindow::OnMouseLeave(){}

void AudioCaptureWindow::OnFocus(){
    focus = true;
}

void AudioCaptureWindow::OnKillFocus(){
    focus = false;
}

void AudioCaptureWindow::OnMouseWheel(int delta){}

void AudioCaptureWindow::OnMenuAccel(int id, bool accel){}

void AudioCaptureWindow::OnDropFileA(const char* path){}

void AudioCaptureWindow::OnDropFileW(const wchar_t* path){}

void AudioCaptureWindow::Launch(){
    if (capture){
        DebugError("AudioCaptureWindow::Launch Already Launched");
        return;
    }

    capDev = alcCaptureOpenDevice(NULL, freq, AL_FORMAT_MONO16, 1 << (bit + 2));

    if (!capDev){
        DebugError("AudioCaptureWindow::Launch Capture Device Not Found");
        return;
    }

    DebugLog("AudioCaptureWindow::Launch Open Device %p %s", alcGetString(capDev, ALC_DEVICE_SPECIFIER));

    if (!capBuf){
        capBuf = new short[1 << bit];
        memset(capBuf, 0, 1 << (bit + 1));
    }
    if (!recBuf){
        recBuf = new short[1 << bit];
        memset(recBuf, 0, 1 << (bit + 1));

        alGenSources(1, &alSrc);
        alGenBuffers(1 << queueBit, alBuf);
        alSourcePlay(alSrc);
    }
    if (!freqBuf){
        freqBuf = new _Complex float[1 << bit];
    }

    alcCaptureStart(capDev);
    capture = true;

    DebugLog("AudioCaptureWindow::Launch Success");
}

void AudioCaptureWindow::Stop(){
    alcCaptureStop(capDev);
    capture = false;
}

void AudioCaptureWindow::UpdateBuffer(ALvoid* buf, ALsizei size){
    ALint cnt;
    ALint state;

    //DebugLog("AudioCaptureWindow %p Queue Buffer %d %d", this, head, alBuf[head & queueMask]);
    alBufferData(alBuf[head & queueMask], AL_FORMAT_MONO16, buf, size, freq);
    alSourceQueueBuffers(alSrc, 1, &alBuf[(head++) & queueMask]);

    alGetSourcei(alSrc, AL_BUFFERS_PROCESSED, &cnt);
    while (cnt--){
        //DebugLog("AudioCaptureWindow %p Unqueue Buffer %d %d", this, tail, alBuf[tail & queueMask]);
        alSourceUnqueueBuffers(alSrc, 1, &alBuf[(tail++) & queueMask]);
    }

    alGetSourcei(alSrc, AL_SOURCE_STATE, &state);
    if (state != AL_PLAYING){
        alSourcePlay(alSrc);
    }
}
