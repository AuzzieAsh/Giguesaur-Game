//
//  ViewController.m
//  Test
//
//  Created by Local Ash on 6/15/15.
//  Copyright (c) 2015 Local Ash. All rights reserved.
//

#define degToRad(DEG) (float)(DEG * 3.141592653 / 180.0)

#import "OpenGLView.h"

@implementation OpenGLView

typedef struct {
    float Position[3];
    float Color[4];
} Vertex;
/*
const Vertex Vertices[] = {
    {{1, -1, 0}, {1, 0, 0, 1}},
    {{1, 1, 0}, {0, 1, 0, 1}},
    {{-1, 1, 0}, {0, 0, 1, 1}},
    {{-1, -1, 0}, {0, 0, 0, 1}}
};

const GLubyte Indices[] = {
    0, 1, 2,
    2, 3, 0
};
*/

const Vertex Vertices[] = {
    {{1, -1, 0}, {1, 0, 0, 1}},
    {{1, 1, 0}, {0, 1, 0, 1}},
    {{-1, 1, 0}, {0, 0, 1, 1}},
    {{-1, -1, 0}, {1, 1, 0, 1}},
    {{1, -1, -1}, {0, 1, 1, 1}},
    {{1, 1, -1}, {1, 0, 1, 1}},
    {{-1, 1, -1}, {0, 0, 0, 1}},
    {{-1, -1, -1}, {1, 1, 1, 1}}
};

const GLubyte Indices[] = {
    // Front
    0, 1, 2,
    2, 3, 0,
    // Back
    4, 6, 5,
    4, 7, 6,
    // Left
    2, 7, 3,
    7, 6, 2,
    // Right
    0, 4, 1,
    4, 1, 5,
    // Top
    6, 2, 1,
    1, 6, 5,
    // Bottom
    0, 3, 7,
    0, 7, 4
};

+ (Class)layerClass {
    return [CAEAGLLayer class];
}

- (void)setupLayer {
    _eaglLayer = (CAEAGLLayer*) self.layer;
    _eaglLayer.opaque = YES;
}

- (void)setupContext {
    EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
    _context = [[EAGLContext alloc] initWithAPI:api];
    if (!_context) {
        NSLog(@"Failed to initialize OpenGLES 2.0 context");
        exit(1);
    }
    
    if (![EAGLContext setCurrentContext:_context]) {
        NSLog(@"Failed to set current OpenGL context");
        exit(1);
    }
}

- (void)setupRenderBuffer {
    glGenRenderbuffers(1, &_colorRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
    [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];
}

- (void)setupDepthBuffer {
    glGenRenderbuffers(1, &_depthRenderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, self.frame.size.width, self.frame.size.height);
}

- (void)setupFrameBuffer {
    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
                              GL_RENDERBUFFER, _colorRenderBuffer);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderBuffer);
}

- (GLuint)compileShader:(NSString*)shaderName withType:(GLenum)shaderType {
    
    NSString* shaderPath = [[NSBundle mainBundle] pathForResource:shaderName
                                                           ofType:@"glsl"];
    NSError* error;
    NSString* shaderString = [NSString stringWithContentsOfFile:shaderPath
                                                       encoding:NSUTF8StringEncoding error:&error];
    if (!shaderString) {
        NSLog(@"Error loading shader: %@", error.localizedDescription);
        exit(1);
    }
    
    GLuint shaderHandle = glCreateShader(shaderType);
    
    const char * shaderStringUTF8 = [shaderString UTF8String];
    int shaderStringLength = (int)[shaderString length];
    glShaderSource(shaderHandle, 1, &shaderStringUTF8, &shaderStringLength);
    
    glCompileShader(shaderHandle);
    
    GLint compileSuccess;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
    if (compileSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
        NSString *messageString = [NSString stringWithUTF8String:messages];
        NSLog(@"%@", messageString);
        exit(1);
    }
    
    return shaderHandle;
    
}

- (void)compileShaders {
    
    GLuint vertexShader = [self compileShader:@"SimpleVertex"
                                     withType:GL_VERTEX_SHADER];
    GLuint fragmentShader = [self compileShader:@"SimpleFragment"
                                       withType:GL_FRAGMENT_SHADER];
    
    GLuint programHandle = glCreateProgram();
    glAttachShader(programHandle, vertexShader);
    glAttachShader(programHandle, fragmentShader);
    glLinkProgram(programHandle);
    
    GLint linkSuccess;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
    if (linkSuccess == GL_FALSE) {
        GLchar messages[256];
        glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
        NSString *messageString = [NSString stringWithUTF8String:messages];
        NSLog(@"%@", messageString);
        exit(1);
    }
    
    glUseProgram(programHandle);
    
    _positionSlot = glGetAttribLocation(programHandle, "Position");
    _colorSlot = glGetAttribLocation(programHandle, "SourceColor");
    glEnableVertexAttribArray(_positionSlot);
    glEnableVertexAttribArray(_colorSlot);
    _projectionUniform = glGetUniformLocation(programHandle, "Projection");
    _modelViewUniform = glGetUniformLocation(programHandle, "Modelview");
}

- (void)setupVBOs {
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
    
    GLuint indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}

/***** DRAW CODE *****/
- (void)render:(CADisplayLink*)displayLink {
    glClearColor(0, 0.5, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    float h = 4.0f * self.frame.size.height / self.frame.size.width;
    float left = -2;
    float right = 2;
    float bottom = -h/2;
    float top = h/2;
    float near = 4;
    float far = 10;
    
    // Frustrum Projection
    float projection[] = {
        (2*near)/(right-left), 0, (right+left)/(right-left), 0,
        0, (2*near)/(top-bottom), (top+bottom)/(top-bottom), 0,
        0, 0, -(far+near)/(far-near), -2*(far*near)/(far-near),
        0, 0, -1, 0
    };
    
    // Translate
    float x = sin(CACurrentMediaTime());
    float y = cos(CACurrentMediaTime());
    float z = -3;

    float translation[] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    };
    
    //Rotation
    _currentRotation += displayLink.duration * 90;
    
    float xRadians = degToRad(_currentRotation);
    float yRadians = degToRad(_currentRotation);
    float zRadians = degToRad(0);
    
    /*
         |  cycz + sxsysz   czsxsy - cysz   cxsy  0 |
     M = |  cxsz            cxcz           -sx    0 |
         |  cysxsz - czsy   cyczsx + sysz   cxcy  0 |
         |  0               0               0     1 |
     
        where cA = cos(A), sA = sin(A) for A = x,y,z
     */
    
    float cx = cosf(xRadians);
    float sx = sinf(xRadians);
    float cy = cosf(yRadians);
    float sy = sinf(yRadians);
    float cz = cosf(zRadians);
    float sz = sinf(zRadians);
    
    float rotation[] = {
        (cy * cz) + (sx * sy * sz), cx * sz, (cy * sx * sz) - (cz * sy), 0.0,
        (cz * sx * sy) - (cy * sz), cx * cz, (cy * cz * sx) + (sy * sz), 0.0,
        cx * sy, -sx, cx * cy, 0.0,
        0.0, 0.0, 0.0, 1.0
    };
    
    // Calculate modelView matrix (translation * rotation)
    float modelView[16];
    modelView[0] = translation[0] * rotation[0] + translation[4] * rotation[1] + translation[8] * rotation[2] + translation[12] * rotation[3];
    modelView[1] = translation[1] * rotation[0] + translation[5] * rotation[1] + translation[9] * rotation[2] + translation[13] * rotation[3];
    modelView[2] = translation[2] * rotation[0] + translation[6] * rotation[1] + translation[10] * rotation[2] + translation[14] * rotation[3];
    modelView[3] = translation[3] * rotation[0] + translation[7] * rotation[1] + translation[11] * rotation[2] + translation[15] * rotation[3];
    
    modelView[4] = translation[0] * rotation[4] + translation[4] * rotation[5] + translation[8] * rotation[6] + translation[12] * rotation[7];
    modelView[5] = translation[1] * rotation[4] + translation[5] * rotation[5] + translation[9] * rotation[6] + translation[13] * rotation[7];
    modelView[6] = translation[2] * rotation[4] + translation[6] * rotation[5] + translation[10] * rotation[6] + translation[14] * rotation[7];
    modelView[7] = translation[3] * rotation[4] + translation[7] * rotation[5] + translation[11] * rotation[6] + translation[15] * rotation[7];
    
    modelView[8] = translation[0] * rotation[8] + translation[4] * rotation[9] + translation[8] * rotation[10] + translation[12] * rotation[11];
    modelView[9] = translation[1] * rotation[8] + translation[5] * rotation[9] + translation[9] * rotation[10] + translation[13] * rotation[11];
    modelView[10] = translation[2] * rotation[8] + translation[6] * rotation[9] + translation[10] * rotation[10] + translation[14] * rotation[11];
    modelView[11] = translation[3] * rotation[8] + translation[7] * rotation[9] + translation[11] * rotation[10] + translation[15] * rotation[11];
    
    modelView[12] = translation[0] * rotation[12] + translation[4] * rotation[13] + translation[8] * rotation[14] + translation[12] * rotation[15];
    modelView[13] = translation[1] * rotation[12] + translation[5] * rotation[13] + translation[9] * rotation[14] + translation[13] * rotation[15];
    modelView[14] = translation[2] * rotation[12] + translation[6] * rotation[13] + translation[10] * rotation[14] + translation[14] * rotation[15];
    modelView[15] = translation[3] * rotation[12] + translation[7] * rotation[13] + translation[11] * rotation[14] + translation[15] * rotation[15];
    
    glUniformMatrix4fv(_projectionUniform, 1, 0, projection);
    glUniformMatrix4fv(_modelViewUniform, 1, 0, modelView);
    
    glViewport(0, 0, self.frame.size.width, self.frame.size.height);
    
    glVertexAttribPointer(_positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
    glVertexAttribPointer(_colorSlot, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          (GLvoid*) (sizeof(float) * 3));
    
    glDrawElements(GL_TRIANGLES, sizeof(Indices)/sizeof(Indices[0]), GL_UNSIGNED_BYTE, 0);
    
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

- (void)setupDisplayLink {
    CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        [self setupLayer];
        [self setupContext];
        [self setupDepthBuffer];
        [self setupRenderBuffer];
        [self setupFrameBuffer];
        [self compileShaders];
        [self setupVBOs];
        [self setupDisplayLink];
    }
    return self;
}

@end
