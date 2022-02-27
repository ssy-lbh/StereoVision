#include <glfunc.h>

#include <gl/wgl.h>

static bool init = false;

#define GL_GETFUNC(x) (x = (decltype(x))wglGetProcAddress(#x))

PFNGLCOLORMASKIPROC glColorMaski;
PFNGLGETBOOLEANI_VPROC glGetBooleani_v;
PFNGLGETINTEGERI_VPROC glGetIntegeri_v;
PFNGLENABLEIPROC glEnablei;
PFNGLDISABLEIPROC glDisablei;
PFNGLISENABLEDIPROC glIsEnabledi;
PFNGLBEGINTRANSFORMFEEDBACKPROC glBeginTransformFeedback;
PFNGLENDTRANSFORMFEEDBACKPROC glEndTransformFeedback;
PFNGLBINDBUFFERRANGEPROC glBindBufferRange;
PFNGLBINDBUFFERBASEPROC glBindBufferBase;
PFNGLTRANSFORMFEEDBACKVARYINGSPROC glTransformFeedbackVaryings;
PFNGLGETTRANSFORMFEEDBACKVARYINGPROC glGetTransformFeedbackVarying;
PFNGLCLAMPCOLORPROC glClampColor;
PFNGLBEGINCONDITIONALRENDERPROC glBeginConditionalRender;
PFNGLENDCONDITIONALRENDERPROC glEndConditionalRender;
PFNGLVERTEXATTRIBIPOINTERPROC glVertexAttribIPointer;
PFNGLGETVERTEXATTRIBIIVPROC glGetVertexAttribIiv;
PFNGLGETVERTEXATTRIBIUIVPROC glGetVertexAttribIuiv;
PFNGLVERTEXATTRIBI1IPROC glVertexAttribI1i;
PFNGLVERTEXATTRIBI2IPROC glVertexAttribI2i;
PFNGLVERTEXATTRIBI3IPROC glVertexAttribI3i;
PFNGLVERTEXATTRIBI4IPROC glVertexAttribI4i;
PFNGLVERTEXATTRIBI1UIPROC glVertexAttribI1ui;
PFNGLVERTEXATTRIBI2UIPROC glVertexAttribI2ui;
PFNGLVERTEXATTRIBI3UIPROC glVertexAttribI3ui;
PFNGLVERTEXATTRIBI4UIPROC glVertexAttribI4ui;
PFNGLVERTEXATTRIBI1IVPROC glVertexAttribI1iv;
PFNGLVERTEXATTRIBI2IVPROC glVertexAttribI2iv;
PFNGLVERTEXATTRIBI3IVPROC glVertexAttribI3iv;
PFNGLVERTEXATTRIBI4IVPROC glVertexAttribI4iv;
PFNGLVERTEXATTRIBI1UIVPROC glVertexAttribI1uiv;
PFNGLVERTEXATTRIBI2UIVPROC glVertexAttribI2uiv;
PFNGLVERTEXATTRIBI3UIVPROC glVertexAttribI3uiv;
PFNGLVERTEXATTRIBI4UIVPROC glVertexAttribI4uiv;
PFNGLVERTEXATTRIBI4BVPROC glVertexAttribI4bv;
PFNGLVERTEXATTRIBI4SVPROC glVertexAttribI4sv;
PFNGLVERTEXATTRIBI4UBVPROC glVertexAttribI4ubv;
PFNGLVERTEXATTRIBI4USVPROC glVertexAttribI4usv;
PFNGLGETUNIFORMUIVPROC glGetUniformuiv;
PFNGLBINDFRAGDATALOCATIONPROC glBindFragDataLocation;
PFNGLGETFRAGDATALOCATIONPROC glGetFragDataLocation;
PFNGLUNIFORM1UIPROC glUniform1ui;
PFNGLUNIFORM2UIPROC glUniform2ui;
PFNGLUNIFORM3UIPROC glUniform3ui;
PFNGLUNIFORM4UIPROC glUniform4ui;
PFNGLUNIFORM1UIVPROC glUniform1uiv;
PFNGLUNIFORM2UIVPROC glUniform2uiv;
PFNGLUNIFORM3UIVPROC glUniform3uiv;
PFNGLUNIFORM4UIVPROC glUniform4uiv;
PFNGLTEXPARAMETERIIVPROC glTexParameterIiv;
PFNGLTEXPARAMETERIUIVPROC glTexParameterIuiv;
PFNGLGETTEXPARAMETERIIVPROC glGetTexParameterIiv;
PFNGLGETTEXPARAMETERIUIVPROC glGetTexParameterIuiv;
PFNGLCLEARBUFFERIVPROC glClearBufferiv;
PFNGLCLEARBUFFERUIVPROC glClearBufferuiv;
PFNGLCLEARBUFFERFVPROC glClearBufferfv;
PFNGLCLEARBUFFERFIPROC glClearBufferfi;
PFNGLGETSTRINGIPROC glGetStringi;
PFNGLISRENDERBUFFERPROC glIsRenderbuffer;
PFNGLBINDRENDERBUFFERPROC glBindRenderbuffer;
PFNGLDELETERENDERBUFFERSPROC glDeleteRenderbuffers;
PFNGLGENRENDERBUFFERSPROC glGenRenderbuffers;
PFNGLRENDERBUFFERSTORAGEPROC glRenderbufferStorage;
PFNGLGETRENDERBUFFERPARAMETERIVPROC glGetRenderbufferParameteriv;
PFNGLISFRAMEBUFFERPROC glIsFramebuffer;
PFNGLBINDFRAMEBUFFERPROC glBindFramebuffer;
PFNGLDELETEFRAMEBUFFERSPROC glDeleteFramebuffers;
PFNGLGENFRAMEBUFFERSPROC glGenFramebuffers;
PFNGLCHECKFRAMEBUFFERSTATUSPROC glCheckFramebufferStatus;
PFNGLFRAMEBUFFERTEXTURE1DPROC glFramebufferTexture1D;
PFNGLFRAMEBUFFERTEXTURE2DPROC glFramebufferTexture2D;
PFNGLFRAMEBUFFERTEXTURE3DPROC glFramebufferTexture3D;
PFNGLFRAMEBUFFERRENDERBUFFERPROC glFramebufferRenderbuffer;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVPROC glGetFramebufferAttachmentParameteriv;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap;
PFNGLBLITFRAMEBUFFERPROC glBlitFramebuffer;
PFNGLRENDERBUFFERSTORAGEMULTISAMPLEPROC glRenderbufferStorageMultisample;
PFNGLFRAMEBUFFERTEXTURELAYERPROC glFramebufferTextureLayer;
PFNGLMAPBUFFERRANGEPROC glMapBufferRange;
PFNGLFLUSHMAPPEDBUFFERRANGEPROC glFlushMappedBufferRange;
PFNGLBINDVERTEXARRAYPROC glBindVertexArray;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays;
PFNGLGENVERTEXARRAYSPROC glGenVertexArrays;
PFNGLISVERTEXARRAYPROC glIsVertexArray;

PFNGLDRAWARRAYSINSTANCEDBASEINSTANCEPROC glDrawArraysInstancedBaseInstance;
PFNGLDRAWELEMENTSINSTANCEDBASEINSTANCEPROC glDrawElementsInstancedBaseInstance;
PFNGLDRAWELEMENTSINSTANCEDBASEVERTEXBASEINSTANCEPROC glDrawElementsInstancedBaseVertexBaseInstance;
PFNGLGETINTERNALFORMATIVPROC glGetInternalformativ;
PFNGLGETACTIVEATOMICCOUNTERBUFFERIVPROC glGetActiveAtomicCounterBufferiv;
PFNGLBINDIMAGETEXTUREPROC glBindImageTexture;
PFNGLMEMORYBARRIERPROC glMemoryBarrier;
PFNGLTEXSTORAGE1DPROC glTexStorage1D;
PFNGLTEXSTORAGE2DPROC glTexStorage2D;
PFNGLTEXSTORAGE3DPROC glTexStorage3D;
PFNGLDRAWTRANSFORMFEEDBACKINSTANCEDPROC glDrawTransformFeedbackInstanced;
PFNGLDRAWTRANSFORMFEEDBACKSTREAMINSTANCEDPROC glDrawTransformFeedbackStreamInstanced;

PFNGLBLENDEQUATIONSEPARATEPROC glBlendEquationSeparate;
PFNGLDRAWBUFFERSPROC glDrawBuffers;
PFNGLSTENCILOPSEPARATEPROC glStencilOpSeparate;
PFNGLSTENCILFUNCSEPARATEPROC glStencilFuncSeparate;
PFNGLSTENCILMASKSEPARATEPROC glStencilMaskSeparate;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLCREATESHADERPROC glCreateShader;
PFNGLDELETEPROGRAMPROC glDeleteProgram;
PFNGLDELETESHADERPROC glDeleteShader;
PFNGLDETACHSHADERPROC glDetachShader;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLGETACTIVEATTRIBPROC glGetActiveAttrib;
PFNGLGETACTIVEUNIFORMPROC glGetActiveUniform;
PFNGLGETATTACHEDSHADERSPROC glGetAttachedShaders;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLGETSHADERINFOLOGPROC glGetShaderInfoLog;
PFNGLGETSHADERSOURCEPROC glGetShaderSource;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLGETUNIFORMFVPROC glGetUniformfv;
PFNGLGETUNIFORMIVPROC glGetUniformiv;
PFNGLGETVERTEXATTRIBDVPROC glGetVertexAttribdv;
PFNGLGETVERTEXATTRIBFVPROC glGetVertexAttribfv;
PFNGLGETVERTEXATTRIBIVPROC glGetVertexAttribiv;
PFNGLGETVERTEXATTRIBPOINTERVPROC glGetVertexAttribPointerv;
PFNGLISPROGRAMPROC glIsProgram;
PFNGLISSHADERPROC glIsShader;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLUNIFORM1FPROC glUniform1f;
PFNGLUNIFORM2FPROC glUniform2f;
PFNGLUNIFORM3FPROC glUniform3f;
PFNGLUNIFORM4FPROC glUniform4f;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLUNIFORM2IPROC glUniform2i;
PFNGLUNIFORM3IPROC glUniform3i;
PFNGLUNIFORM4IPROC glUniform4i;
PFNGLUNIFORM1FVPROC glUniform1fv;
PFNGLUNIFORM2FVPROC glUniform2fv;
PFNGLUNIFORM3FVPROC glUniform3fv;
PFNGLUNIFORM4FVPROC glUniform4fv;
PFNGLUNIFORM1IVPROC glUniform1iv;
PFNGLUNIFORM2IVPROC glUniform2iv;
PFNGLUNIFORM3IVPROC glUniform3iv;
PFNGLUNIFORM4IVPROC glUniform4iv;
PFNGLUNIFORMMATRIX2FVPROC glUniformMatrix2fv;
PFNGLUNIFORMMATRIX3FVPROC glUniformMatrix3fv;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLVALIDATEPROGRAMPROC glValidateProgram;
PFNGLVERTEXATTRIB1DPROC glVertexAttrib1d;
PFNGLVERTEXATTRIB1DVPROC glVertexAttrib1dv;
PFNGLVERTEXATTRIB1FPROC glVertexAttrib1f;
PFNGLVERTEXATTRIB1FVPROC glVertexAttrib1fv;
PFNGLVERTEXATTRIB1SPROC glVertexAttrib1s;
PFNGLVERTEXATTRIB1SVPROC glVertexAttrib1sv;
PFNGLVERTEXATTRIB2DPROC glVertexAttrib2d;
PFNGLVERTEXATTRIB2DVPROC glVertexAttrib2dv;
PFNGLVERTEXATTRIB2FPROC glVertexAttrib2f;
PFNGLVERTEXATTRIB2FVPROC glVertexAttrib2fv;
PFNGLVERTEXATTRIB2SPROC glVertexAttrib2s;
PFNGLVERTEXATTRIB2SVPROC glVertexAttrib2sv;
PFNGLVERTEXATTRIB3DPROC glVertexAttrib3d;
PFNGLVERTEXATTRIB3DVPROC glVertexAttrib3dv;
PFNGLVERTEXATTRIB3FPROC glVertexAttrib3f;
PFNGLVERTEXATTRIB3FVPROC glVertexAttrib3fv;
PFNGLVERTEXATTRIB3SPROC glVertexAttrib3s;
PFNGLVERTEXATTRIB3SVPROC glVertexAttrib3sv;
PFNGLVERTEXATTRIB4NBVPROC glVertexAttrib4Nbv;
PFNGLVERTEXATTRIB4NIVPROC glVertexAttrib4Niv;
PFNGLVERTEXATTRIB4NSVPROC glVertexAttrib4Nsv;
PFNGLVERTEXATTRIB4NUBPROC glVertexAttrib4Nub;
PFNGLVERTEXATTRIB4NUBVPROC glVertexAttrib4Nubv;
PFNGLVERTEXATTRIB4NUIVPROC glVertexAttrib4Nuiv;
PFNGLVERTEXATTRIB4NUSVPROC glVertexAttrib4Nusv;
PFNGLVERTEXATTRIB4BVPROC glVertexAttrib4bv;
PFNGLVERTEXATTRIB4DPROC glVertexAttrib4d;
PFNGLVERTEXATTRIB4DVPROC glVertexAttrib4dv;
PFNGLVERTEXATTRIB4FPROC glVertexAttrib4f;
PFNGLVERTEXATTRIB4FVPROC glVertexAttrib4fv;
PFNGLVERTEXATTRIB4IVPROC glVertexAttrib4iv;
PFNGLVERTEXATTRIB4SPROC glVertexAttrib4s;
PFNGLVERTEXATTRIB4SVPROC glVertexAttrib4sv;
PFNGLVERTEXATTRIB4UBVPROC glVertexAttrib4ubv;
PFNGLVERTEXATTRIB4UIVPROC glVertexAttrib4uiv;
PFNGLVERTEXATTRIB4USVPROC glVertexAttrib4usv;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

PFNGLCLEARBUFFERDATAPROC glClearBufferData;
PFNGLCLEARBUFFERSUBDATAPROC glClearBufferSubData;
PFNGLDISPATCHCOMPUTEPROC glDispatchCompute;
PFNGLDISPATCHCOMPUTEINDIRECTPROC glDispatchComputeIndirect;
PFNGLCOPYIMAGESUBDATAPROC glCopyImageSubData;
PFNGLFRAMEBUFFERPARAMETERIPROC glFramebufferParameteri;
PFNGLGETFRAMEBUFFERPARAMETERIVPROC glGetFramebufferParameteriv;
PFNGLGETINTERNALFORMATI64VPROC glGetInternalformati64v;
PFNGLINVALIDATETEXSUBIMAGEPROC glInvalidateTexSubImage;
PFNGLINVALIDATETEXIMAGEPROC glInvalidateTexImage;
PFNGLINVALIDATEBUFFERSUBDATAPROC glInvalidateBufferSubData;
PFNGLINVALIDATEBUFFERDATAPROC glInvalidateBufferData;
PFNGLINVALIDATEFRAMEBUFFERPROC glInvalidateFramebuffer;
PFNGLINVALIDATESUBFRAMEBUFFERPROC glInvalidateSubFramebuffer;
PFNGLMULTIDRAWARRAYSINDIRECTPROC glMultiDrawArraysIndirect;
PFNGLMULTIDRAWELEMENTSINDIRECTPROC glMultiDrawElementsIndirect;
PFNGLGETPROGRAMINTERFACEIVPROC glGetProgramInterfaceiv;
PFNGLGETPROGRAMRESOURCEINDEXPROC glGetProgramResourceIndex;
PFNGLGETPROGRAMRESOURCENAMEPROC glGetProgramResourceName;
PFNGLGETPROGRAMRESOURCEIVPROC glGetProgramResourceiv;
PFNGLGETPROGRAMRESOURCELOCATIONPROC glGetProgramResourceLocation;
PFNGLGETPROGRAMRESOURCELOCATIONINDEXPROC glGetProgramResourceLocationIndex;
PFNGLSHADERSTORAGEBLOCKBINDINGPROC glShaderStorageBlockBinding;
PFNGLTEXBUFFERRANGEPROC glTexBufferRange;
PFNGLTEXSTORAGE2DMULTISAMPLEPROC glTexStorage2DMultisample;
PFNGLTEXSTORAGE3DMULTISAMPLEPROC glTexStorage3DMultisample;
PFNGLTEXTUREVIEWPROC glTextureView;
PFNGLBINDVERTEXBUFFERPROC glBindVertexBuffer;
PFNGLVERTEXATTRIBFORMATPROC glVertexAttribFormat;
PFNGLVERTEXATTRIBIFORMATPROC glVertexAttribIFormat;
PFNGLVERTEXATTRIBLFORMATPROC glVertexAttribLFormat;
PFNGLVERTEXATTRIBBINDINGPROC glVertexAttribBinding;
PFNGLVERTEXBINDINGDIVISORPROC glVertexBindingDivisor;
PFNGLDEBUGMESSAGECONTROLPROC glDebugMessageControl;
PFNGLDEBUGMESSAGEINSERTPROC glDebugMessageInsert;
PFNGLDEBUGMESSAGECALLBACKPROC glDebugMessageCallback;
PFNGLGETDEBUGMESSAGELOGPROC glGetDebugMessageLog;
PFNGLPUSHDEBUGGROUPPROC glPushDebugGroup;
PFNGLPOPDEBUGGROUPPROC glPopDebugGroup;
PFNGLOBJECTLABELPROC glObjectLabel;
PFNGLGETOBJECTLABELPROC glGetObjectLabel;
PFNGLOBJECTPTRLABELPROC glObjectPtrLabel;
PFNGLGETOBJECTPTRLABELPROC glGetObjectPtrLabel;

PFNGLRELEASESHADERCOMPILERPROC glReleaseShaderCompiler;
PFNGLSHADERBINARYPROC glShaderBinary;
PFNGLGETSHADERPRECISIONFORMATPROC glGetShaderPrecisionFormat;
PFNGLDEPTHRANGEFPROC glDepthRangef;
PFNGLCLEARDEPTHFPROC glClearDepthf;
PFNGLGETPROGRAMBINARYPROC glGetProgramBinary;
PFNGLPROGRAMBINARYPROC glProgramBinary;
PFNGLPROGRAMPARAMETERIPROC glProgramParameteri;
PFNGLUSEPROGRAMSTAGESPROC glUseProgramStages;
PFNGLACTIVESHADERPROGRAMPROC glActiveShaderProgram;
PFNGLCREATESHADERPROGRAMVPROC glCreateShaderProgramv;
PFNGLBINDPROGRAMPIPELINEPROC glBindProgramPipeline;
PFNGLDELETEPROGRAMPIPELINESPROC glDeleteProgramPipelines;
PFNGLGENPROGRAMPIPELINESPROC glGenProgramPipelines;
PFNGLISPROGRAMPIPELINEPROC glIsProgramPipeline;
PFNGLGETPROGRAMPIPELINEIVPROC glGetProgramPipelineiv;
PFNGLPROGRAMUNIFORM1IPROC glProgramUniform1i;
PFNGLPROGRAMUNIFORM1IVPROC glProgramUniform1iv;
PFNGLPROGRAMUNIFORM1FPROC glProgramUniform1f;
PFNGLPROGRAMUNIFORM1FVPROC glProgramUniform1fv;
PFNGLPROGRAMUNIFORM1DPROC glProgramUniform1d;
PFNGLPROGRAMUNIFORM1DVPROC glProgramUniform1dv;
PFNGLPROGRAMUNIFORM1UIPROC glProgramUniform1ui;
PFNGLPROGRAMUNIFORM1UIVPROC glProgramUniform1uiv;
PFNGLPROGRAMUNIFORM2IPROC glProgramUniform2i;
PFNGLPROGRAMUNIFORM2IVPROC glProgramUniform2iv;
PFNGLPROGRAMUNIFORM2FPROC glProgramUniform2f;
PFNGLPROGRAMUNIFORM2FVPROC glProgramUniform2fv;
PFNGLPROGRAMUNIFORM2DPROC glProgramUniform2d;
PFNGLPROGRAMUNIFORM2DVPROC glProgramUniform2dv;
PFNGLPROGRAMUNIFORM2UIPROC glProgramUniform2ui;
PFNGLPROGRAMUNIFORM2UIVPROC glProgramUniform2uiv;
PFNGLPROGRAMUNIFORM3IPROC glProgramUniform3i;
PFNGLPROGRAMUNIFORM3IVPROC glProgramUniform3iv;
PFNGLPROGRAMUNIFORM3FPROC glProgramUniform3f;
PFNGLPROGRAMUNIFORM3FVPROC glProgramUniform3fv;
PFNGLPROGRAMUNIFORM3DPROC glProgramUniform3d;
PFNGLPROGRAMUNIFORM3DVPROC glProgramUniform3dv;
PFNGLPROGRAMUNIFORM3UIPROC glProgramUniform3ui;
PFNGLPROGRAMUNIFORM3UIVPROC glProgramUniform3uiv;
PFNGLPROGRAMUNIFORM4IPROC glProgramUniform4i;
PFNGLPROGRAMUNIFORM4IVPROC glProgramUniform4iv;
PFNGLPROGRAMUNIFORM4FPROC glProgramUniform4f;
PFNGLPROGRAMUNIFORM4FVPROC glProgramUniform4fv;
PFNGLPROGRAMUNIFORM4DPROC glProgramUniform4d;
PFNGLPROGRAMUNIFORM4DVPROC glProgramUniform4dv;
PFNGLPROGRAMUNIFORM4UIPROC glProgramUniform4ui;
PFNGLPROGRAMUNIFORM4UIVPROC glProgramUniform4uiv;
PFNGLPROGRAMUNIFORMMATRIX2FVPROC glProgramUniformMatrix2fv;
PFNGLPROGRAMUNIFORMMATRIX3FVPROC glProgramUniformMatrix3fv;
PFNGLPROGRAMUNIFORMMATRIX4FVPROC glProgramUniformMatrix4fv;
PFNGLPROGRAMUNIFORMMATRIX2DVPROC glProgramUniformMatrix2dv;
PFNGLPROGRAMUNIFORMMATRIX3DVPROC glProgramUniformMatrix3dv;
PFNGLPROGRAMUNIFORMMATRIX4DVPROC glProgramUniformMatrix4dv;
PFNGLPROGRAMUNIFORMMATRIX2X3FVPROC glProgramUniformMatrix2x3fv;
PFNGLPROGRAMUNIFORMMATRIX3X2FVPROC glProgramUniformMatrix3x2fv;
PFNGLPROGRAMUNIFORMMATRIX2X4FVPROC glProgramUniformMatrix2x4fv;
PFNGLPROGRAMUNIFORMMATRIX4X2FVPROC glProgramUniformMatrix4x2fv;
PFNGLPROGRAMUNIFORMMATRIX3X4FVPROC glProgramUniformMatrix3x4fv;
PFNGLPROGRAMUNIFORMMATRIX4X3FVPROC glProgramUniformMatrix4x3fv;
PFNGLPROGRAMUNIFORMMATRIX2X3DVPROC glProgramUniformMatrix2x3dv;
PFNGLPROGRAMUNIFORMMATRIX3X2DVPROC glProgramUniformMatrix3x2dv;
PFNGLPROGRAMUNIFORMMATRIX2X4DVPROC glProgramUniformMatrix2x4dv;
PFNGLPROGRAMUNIFORMMATRIX4X2DVPROC glProgramUniformMatrix4x2dv;
PFNGLPROGRAMUNIFORMMATRIX3X4DVPROC glProgramUniformMatrix3x4dv;
PFNGLPROGRAMUNIFORMMATRIX4X3DVPROC glProgramUniformMatrix4x3dv;
PFNGLVALIDATEPROGRAMPIPELINEPROC glValidateProgramPipeline;
PFNGLGETPROGRAMPIPELINEINFOLOGPROC glGetProgramPipelineInfoLog;
PFNGLVERTEXATTRIBL1DPROC glVertexAttribL1d;
PFNGLVERTEXATTRIBL2DPROC glVertexAttribL2d;
PFNGLVERTEXATTRIBL3DPROC glVertexAttribL3d;
PFNGLVERTEXATTRIBL4DPROC glVertexAttribL4d;
PFNGLVERTEXATTRIBL1DVPROC glVertexAttribL1dv;
PFNGLVERTEXATTRIBL2DVPROC glVertexAttribL2dv;
PFNGLVERTEXATTRIBL3DVPROC glVertexAttribL3dv;
PFNGLVERTEXATTRIBL4DVPROC glVertexAttribL4dv;
PFNGLVERTEXATTRIBLPOINTERPROC glVertexAttribLPointer;
PFNGLGETVERTEXATTRIBLDVPROC glGetVertexAttribLdv;
PFNGLVIEWPORTARRAYVPROC glViewportArrayv;
PFNGLVIEWPORTINDEXEDFPROC glViewportIndexedf;
PFNGLVIEWPORTINDEXEDFVPROC glViewportIndexedfv;
PFNGLSCISSORARRAYVPROC glScissorArrayv;
PFNGLSCISSORINDEXEDPROC glScissorIndexed;
PFNGLSCISSORINDEXEDVPROC glScissorIndexedv;
PFNGLDEPTHRANGEARRAYVPROC glDepthRangeArrayv;
PFNGLDEPTHRANGEINDEXEDPROC glDepthRangeIndexed;
PFNGLGETFLOATI_VPROC glGetFloati_v;
PFNGLGETDOUBLEI_VPROC glGetDoublei_v;

void glInitEXTFunctions(){
    if (init)
        return;

    init = true;

    GL_GETFUNC(glColorMaski);
    GL_GETFUNC(glGetBooleani_v);
    GL_GETFUNC(glGetIntegeri_v);
    GL_GETFUNC(glEnablei);
    GL_GETFUNC(glDisablei);
    GL_GETFUNC(glIsEnabledi);
    GL_GETFUNC(glBeginTransformFeedback);
    GL_GETFUNC(glEndTransformFeedback);
    GL_GETFUNC(glBindBufferRange);
    GL_GETFUNC(glBindBufferBase);
    GL_GETFUNC(glTransformFeedbackVaryings);
    GL_GETFUNC(glGetTransformFeedbackVarying);
    GL_GETFUNC(glClampColor);
    GL_GETFUNC(glBeginConditionalRender);
    GL_GETFUNC(glEndConditionalRender);
    GL_GETFUNC(glVertexAttribIPointer);
    GL_GETFUNC(glGetVertexAttribIiv);
    GL_GETFUNC(glGetVertexAttribIuiv);
    GL_GETFUNC(glVertexAttribI1i);
    GL_GETFUNC(glVertexAttribI2i);
    GL_GETFUNC(glVertexAttribI3i);
    GL_GETFUNC(glVertexAttribI4i);
    GL_GETFUNC(glVertexAttribI1ui);
    GL_GETFUNC(glVertexAttribI2ui);
    GL_GETFUNC(glVertexAttribI3ui);
    GL_GETFUNC(glVertexAttribI4ui);
    GL_GETFUNC(glVertexAttribI1iv);
    GL_GETFUNC(glVertexAttribI2iv);
    GL_GETFUNC(glVertexAttribI3iv);
    GL_GETFUNC(glVertexAttribI4iv);
    GL_GETFUNC(glVertexAttribI1uiv);
    GL_GETFUNC(glVertexAttribI2uiv);
    GL_GETFUNC(glVertexAttribI3uiv);
    GL_GETFUNC(glVertexAttribI4uiv);
    GL_GETFUNC(glVertexAttribI4bv);
    GL_GETFUNC(glVertexAttribI4sv);
    GL_GETFUNC(glVertexAttribI4ubv);
    GL_GETFUNC(glVertexAttribI4usv);
    GL_GETFUNC(glGetUniformuiv);
    GL_GETFUNC(glBindFragDataLocation);
    GL_GETFUNC(glGetFragDataLocation);
    GL_GETFUNC(glUniform1ui);
    GL_GETFUNC(glUniform2ui);
    GL_GETFUNC(glUniform3ui);
    GL_GETFUNC(glUniform4ui);
    GL_GETFUNC(glUniform1uiv);
    GL_GETFUNC(glUniform2uiv);
    GL_GETFUNC(glUniform3uiv);
    GL_GETFUNC(glUniform4uiv);
    GL_GETFUNC(glTexParameterIiv);
    GL_GETFUNC(glTexParameterIuiv);
    GL_GETFUNC(glGetTexParameterIiv);
    GL_GETFUNC(glGetTexParameterIuiv);
    GL_GETFUNC(glClearBufferiv);
    GL_GETFUNC(glClearBufferuiv);
    GL_GETFUNC(glClearBufferfv);
    GL_GETFUNC(glClearBufferfi);
    GL_GETFUNC(glGetStringi);
    GL_GETFUNC(glIsRenderbuffer);
    GL_GETFUNC(glBindRenderbuffer);
    GL_GETFUNC(glDeleteRenderbuffers);
    GL_GETFUNC(glGenRenderbuffers);
    GL_GETFUNC(glRenderbufferStorage);
    GL_GETFUNC(glGetRenderbufferParameteriv);
    GL_GETFUNC(glIsFramebuffer);
    GL_GETFUNC(glBindFramebuffer);
    GL_GETFUNC(glDeleteFramebuffers);
    GL_GETFUNC(glGenFramebuffers);
    GL_GETFUNC(glCheckFramebufferStatus);
    GL_GETFUNC(glFramebufferTexture1D);
    GL_GETFUNC(glFramebufferTexture2D);
    GL_GETFUNC(glFramebufferTexture3D);
    GL_GETFUNC(glFramebufferRenderbuffer);
    GL_GETFUNC(glGetFramebufferAttachmentParameteriv);
    GL_GETFUNC(glGenerateMipmap);
    GL_GETFUNC(glBlitFramebuffer);
    GL_GETFUNC(glRenderbufferStorageMultisample);
    GL_GETFUNC(glFramebufferTextureLayer);
    GL_GETFUNC(glMapBufferRange);
    GL_GETFUNC(glFlushMappedBufferRange);
    GL_GETFUNC(glBindVertexArray);
    GL_GETFUNC(glDeleteVertexArrays);
    GL_GETFUNC(glGenVertexArrays);
    GL_GETFUNC(glIsVertexArray);

    GL_GETFUNC(glDrawArraysInstancedBaseInstance);
    GL_GETFUNC(glDrawElementsInstancedBaseInstance);
    GL_GETFUNC(glDrawElementsInstancedBaseVertexBaseInstance);
    GL_GETFUNC(glGetInternalformativ);
    GL_GETFUNC(glGetActiveAtomicCounterBufferiv);
    GL_GETFUNC(glBindImageTexture);
    GL_GETFUNC(glMemoryBarrier);
    GL_GETFUNC(glTexStorage1D);
    GL_GETFUNC(glTexStorage2D);
    GL_GETFUNC(glTexStorage3D);
    GL_GETFUNC(glDrawTransformFeedbackInstanced);
    GL_GETFUNC(glDrawTransformFeedbackStreamInstanced);

    GL_GETFUNC(glBlendEquationSeparate);
    GL_GETFUNC(glDrawBuffers);
    GL_GETFUNC(glStencilOpSeparate);
    GL_GETFUNC(glStencilFuncSeparate);
    GL_GETFUNC(glStencilMaskSeparate);
    GL_GETFUNC(glAttachShader);
    GL_GETFUNC(glBindAttribLocation);
    GL_GETFUNC(glCompileShader);
    GL_GETFUNC(glCreateProgram);
    GL_GETFUNC(glCreateShader);
    GL_GETFUNC(glDeleteProgram);
    GL_GETFUNC(glDeleteShader);
    GL_GETFUNC(glDetachShader);
    GL_GETFUNC(glDisableVertexAttribArray);
    GL_GETFUNC(glEnableVertexAttribArray);
    GL_GETFUNC(glGetActiveAttrib);
    GL_GETFUNC(glGetActiveUniform);
    GL_GETFUNC(glGetAttachedShaders);
    GL_GETFUNC(glGetAttribLocation);
    GL_GETFUNC(glGetProgramiv);
    GL_GETFUNC(glGetProgramInfoLog);
    GL_GETFUNC(glGetShaderiv);
    GL_GETFUNC(glGetShaderInfoLog);
    GL_GETFUNC(glGetShaderSource);
    GL_GETFUNC(glGetUniformLocation);
    GL_GETFUNC(glGetUniformfv);
    GL_GETFUNC(glGetUniformiv);
    GL_GETFUNC(glGetVertexAttribdv);
    GL_GETFUNC(glGetVertexAttribfv);
    GL_GETFUNC(glGetVertexAttribiv);
    GL_GETFUNC(glGetVertexAttribPointerv);
    GL_GETFUNC(glIsProgram);
    GL_GETFUNC(glIsShader);
    GL_GETFUNC(glLinkProgram);
    GL_GETFUNC(glShaderSource);
    GL_GETFUNC(glUseProgram);
    GL_GETFUNC(glUniform1f);
    GL_GETFUNC(glUniform2f);
    GL_GETFUNC(glUniform3f);
    GL_GETFUNC(glUniform4f);
    GL_GETFUNC(glUniform1i);
    GL_GETFUNC(glUniform2i);
    GL_GETFUNC(glUniform3i);
    GL_GETFUNC(glUniform4i);
    GL_GETFUNC(glUniform1fv);
    GL_GETFUNC(glUniform2fv);
    GL_GETFUNC(glUniform3fv);
    GL_GETFUNC(glUniform4fv);
    GL_GETFUNC(glUniform1iv);
    GL_GETFUNC(glUniform2iv);
    GL_GETFUNC(glUniform3iv);
    GL_GETFUNC(glUniform4iv);
    GL_GETFUNC(glUniformMatrix2fv);
    GL_GETFUNC(glUniformMatrix3fv);
    GL_GETFUNC(glUniformMatrix4fv);
    GL_GETFUNC(glValidateProgram);
    GL_GETFUNC(glVertexAttrib1d);
    GL_GETFUNC(glVertexAttrib1dv);
    GL_GETFUNC(glVertexAttrib1f);
    GL_GETFUNC(glVertexAttrib1fv);
    GL_GETFUNC(glVertexAttrib1s);
    GL_GETFUNC(glVertexAttrib1sv);
    GL_GETFUNC(glVertexAttrib2d);
    GL_GETFUNC(glVertexAttrib2dv);
    GL_GETFUNC(glVertexAttrib2f);
    GL_GETFUNC(glVertexAttrib2fv);
    GL_GETFUNC(glVertexAttrib2s);
    GL_GETFUNC(glVertexAttrib2sv);
    GL_GETFUNC(glVertexAttrib3d);
    GL_GETFUNC(glVertexAttrib3dv);
    GL_GETFUNC(glVertexAttrib3f);
    GL_GETFUNC(glVertexAttrib3fv);
    GL_GETFUNC(glVertexAttrib3s);
    GL_GETFUNC(glVertexAttrib3sv);
    GL_GETFUNC(glVertexAttrib4Nbv);
    GL_GETFUNC(glVertexAttrib4Niv);
    GL_GETFUNC(glVertexAttrib4Nsv);
    GL_GETFUNC(glVertexAttrib4Nub);
    GL_GETFUNC(glVertexAttrib4Nubv);
    GL_GETFUNC(glVertexAttrib4Nuiv);
    GL_GETFUNC(glVertexAttrib4Nusv);
    GL_GETFUNC(glVertexAttrib4bv);
    GL_GETFUNC(glVertexAttrib4d);
    GL_GETFUNC(glVertexAttrib4dv);
    GL_GETFUNC(glVertexAttrib4f);
    GL_GETFUNC(glVertexAttrib4fv);
    GL_GETFUNC(glVertexAttrib4iv);
    GL_GETFUNC(glVertexAttrib4s);
    GL_GETFUNC(glVertexAttrib4sv);
    GL_GETFUNC(glVertexAttrib4ubv);
    GL_GETFUNC(glVertexAttrib4uiv);
    GL_GETFUNC(glVertexAttrib4usv);
    GL_GETFUNC(glVertexAttribPointer);

    GL_GETFUNC(glClearBufferData);
    GL_GETFUNC(glClearBufferSubData);
    GL_GETFUNC(glDispatchCompute);
    GL_GETFUNC(glDispatchComputeIndirect);
    GL_GETFUNC(glCopyImageSubData);
    GL_GETFUNC(glFramebufferParameteri);
    GL_GETFUNC(glGetFramebufferParameteriv);
    GL_GETFUNC(glGetInternalformati64v);
    GL_GETFUNC(glInvalidateTexSubImage);
    GL_GETFUNC(glInvalidateTexImage);
    GL_GETFUNC(glInvalidateBufferSubData);
    GL_GETFUNC(glInvalidateBufferData);
    GL_GETFUNC(glInvalidateFramebuffer);
    GL_GETFUNC(glInvalidateSubFramebuffer);
    GL_GETFUNC(glMultiDrawArraysIndirect);
    GL_GETFUNC(glMultiDrawElementsIndirect);
    GL_GETFUNC(glGetProgramInterfaceiv);
    GL_GETFUNC(glGetProgramResourceIndex);
    GL_GETFUNC(glGetProgramResourceName);
    GL_GETFUNC(glGetProgramResourceiv);
    GL_GETFUNC(glGetProgramResourceLocation);
    GL_GETFUNC(glGetProgramResourceLocationIndex);
    GL_GETFUNC(glShaderStorageBlockBinding);
    GL_GETFUNC(glTexBufferRange);
    GL_GETFUNC(glTexStorage2DMultisample);
    GL_GETFUNC(glTexStorage3DMultisample);
    GL_GETFUNC(glTextureView);
    GL_GETFUNC(glBindVertexBuffer);
    GL_GETFUNC(glVertexAttribFormat);
    GL_GETFUNC(glVertexAttribIFormat);
    GL_GETFUNC(glVertexAttribLFormat);
    GL_GETFUNC(glVertexAttribBinding);
    GL_GETFUNC(glVertexBindingDivisor);
    GL_GETFUNC(glDebugMessageControl);
    GL_GETFUNC(glDebugMessageInsert);
    GL_GETFUNC(glDebugMessageCallback);
    GL_GETFUNC(glGetDebugMessageLog);
    GL_GETFUNC(glPushDebugGroup);
    GL_GETFUNC(glPopDebugGroup);
    GL_GETFUNC(glObjectLabel);
    GL_GETFUNC(glGetObjectLabel);
    GL_GETFUNC(glObjectPtrLabel);
    GL_GETFUNC(glGetObjectPtrLabel);

    GL_GETFUNC(glReleaseShaderCompiler);
    GL_GETFUNC(glShaderBinary);
    GL_GETFUNC(glGetShaderPrecisionFormat);
    GL_GETFUNC(glDepthRangef);
    GL_GETFUNC(glClearDepthf);
    GL_GETFUNC(glGetProgramBinary);
    GL_GETFUNC(glProgramBinary);
    GL_GETFUNC(glProgramParameteri);
    GL_GETFUNC(glUseProgramStages);
    GL_GETFUNC(glActiveShaderProgram);
    GL_GETFUNC(glCreateShaderProgramv);
    GL_GETFUNC(glBindProgramPipeline);
    GL_GETFUNC(glDeleteProgramPipelines);
    GL_GETFUNC(glGenProgramPipelines);
    GL_GETFUNC(glIsProgramPipeline);
    GL_GETFUNC(glGetProgramPipelineiv);
    GL_GETFUNC(glProgramUniform1i);
    GL_GETFUNC(glProgramUniform1iv);
    GL_GETFUNC(glProgramUniform1f);
    GL_GETFUNC(glProgramUniform1fv);
    GL_GETFUNC(glProgramUniform1d);
    GL_GETFUNC(glProgramUniform1dv);
    GL_GETFUNC(glProgramUniform1ui);
    GL_GETFUNC(glProgramUniform1uiv);
    GL_GETFUNC(glProgramUniform2i);
    GL_GETFUNC(glProgramUniform2iv);
    GL_GETFUNC(glProgramUniform2f);
    GL_GETFUNC(glProgramUniform2fv);
    GL_GETFUNC(glProgramUniform2d);
    GL_GETFUNC(glProgramUniform2dv);
    GL_GETFUNC(glProgramUniform2ui);
    GL_GETFUNC(glProgramUniform2uiv);
    GL_GETFUNC(glProgramUniform3i);
    GL_GETFUNC(glProgramUniform3iv);
    GL_GETFUNC(glProgramUniform3f);
    GL_GETFUNC(glProgramUniform3fv);
    GL_GETFUNC(glProgramUniform3d);
    GL_GETFUNC(glProgramUniform3dv);
    GL_GETFUNC(glProgramUniform3ui);
    GL_GETFUNC(glProgramUniform3uiv);
    GL_GETFUNC(glProgramUniform4i);
    GL_GETFUNC(glProgramUniform4iv);
    GL_GETFUNC(glProgramUniform4f);
    GL_GETFUNC(glProgramUniform4fv);
    GL_GETFUNC(glProgramUniform4d);
    GL_GETFUNC(glProgramUniform4dv);
    GL_GETFUNC(glProgramUniform4ui);
    GL_GETFUNC(glProgramUniform4uiv);
    GL_GETFUNC(glProgramUniformMatrix2fv);
    GL_GETFUNC(glProgramUniformMatrix3fv);
    GL_GETFUNC(glProgramUniformMatrix4fv);
    GL_GETFUNC(glProgramUniformMatrix2dv);
    GL_GETFUNC(glProgramUniformMatrix3dv);
    GL_GETFUNC(glProgramUniformMatrix4dv);
    GL_GETFUNC(glProgramUniformMatrix2x3fv);
    GL_GETFUNC(glProgramUniformMatrix3x2fv);
    GL_GETFUNC(glProgramUniformMatrix2x4fv);
    GL_GETFUNC(glProgramUniformMatrix4x2fv);
    GL_GETFUNC(glProgramUniformMatrix3x4fv);
    GL_GETFUNC(glProgramUniformMatrix4x3fv);
    GL_GETFUNC(glProgramUniformMatrix2x3dv);
    GL_GETFUNC(glProgramUniformMatrix3x2dv);
    GL_GETFUNC(glProgramUniformMatrix2x4dv);
    GL_GETFUNC(glProgramUniformMatrix4x2dv);
    GL_GETFUNC(glProgramUniformMatrix3x4dv);
    GL_GETFUNC(glProgramUniformMatrix4x3dv);
    GL_GETFUNC(glValidateProgramPipeline);
    GL_GETFUNC(glGetProgramPipelineInfoLog);
    GL_GETFUNC(glVertexAttribL1d);
    GL_GETFUNC(glVertexAttribL2d);
    GL_GETFUNC(glVertexAttribL3d);
    GL_GETFUNC(glVertexAttribL4d);
    GL_GETFUNC(glVertexAttribL1dv);
    GL_GETFUNC(glVertexAttribL2dv);
    GL_GETFUNC(glVertexAttribL3dv);
    GL_GETFUNC(glVertexAttribL4dv);
    GL_GETFUNC(glVertexAttribLPointer);
    GL_GETFUNC(glGetVertexAttribLdv);
    GL_GETFUNC(glViewportArrayv);
    GL_GETFUNC(glViewportIndexedf);
    GL_GETFUNC(glViewportIndexedfv);
    GL_GETFUNC(glScissorArrayv);
    GL_GETFUNC(glScissorIndexed);
    GL_GETFUNC(glScissorIndexedv);
    GL_GETFUNC(glDepthRangeArrayv);
    GL_GETFUNC(glDepthRangeIndexed);
    GL_GETFUNC(glGetFloati_v);
    GL_GETFUNC(glGetDoublei_v);
}