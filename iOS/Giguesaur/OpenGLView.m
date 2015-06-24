/*
    File: OpenGLView.m
    Author: Ashley Manson
 
    Does all the OpenGL magic and rendering.
    Note: iPad aspect ratio is 4:3
 */

#import "OpenGLView.h"
#import "SimpleMath.h"
#import "Giguesaur/Puzzle.h"

/***** Global Varibles for the Puzzle *****/
Piece pieces[NUM_OF_PIECES];
int holdingPiece = -1;

typedef struct {
    float Position[3];
    float Colour[4];
} Vertex;

const Vertex Vertices[] = {
    {{1, -1, 0}, {0, 0, 0, 1}},
    {{1, 1, 0}, {0, 0, 0, 1}},
    {{-1, 1, 0}, {0, 0, 0, 1}},
    {{-1, -1, 0}, {0, 0, 0, 1}}
};

const GLubyte Indices[] = {
    0, 1, 2,
    2, 3, 0
};

@implementation OpenGLView

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    UITouch *touch = [[event allTouches] anyObject];
    
    // Get the specific point that was touched
    CGPoint point = [touch locationInView:touch.view];
    
    point.y = BOARD_HIEGHT - point.y;
    printf("%f, %f\n", point.x, point.y);
    
    if (holdingPiece >= 0) {
        printf("Placed piece %i\n", holdingPiece);
        pieces[holdingPiece].x_location = point.x;
        pieces[holdingPiece].y_location = point.y;
        holdingPiece = -1;
    }
    else {
        for (int i = 0; i < NUM_OF_PIECES; i++) {
            if(point.x >= pieces[i].x_location - SIDE_HALF && point.x < pieces[i].x_location + SIDE_HALF) {
                if (point.y >= pieces[i].y_location - SIDE_HALF && point.y < pieces[i].y_location + SIDE_HALF) {
                    printf("Touched piece %i\n", i);
                    holdingPiece = i;
                    i = NUM_OF_PIECES;
                }
            }
        }
    }
}

/***** OpenGL Setup Code *****/
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
    
    GLuint vertexShader = [self compileShader:@"SimpleVertex" withType:GL_VERTEX_SHADER];
    GLuint fragmentShader = [self compileShader:@"SimpleFragment" withType:GL_FRAGMENT_SHADER];
    
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
    // Clear the screen
    glClearColor(0, 0.5, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    
    // Sort out projection Matrix
    //float aspectFrustum = 4.0f * BOARD_HIEGHT / BOARD_WIDTH;
    //float aspect = BOARD_HIEGHT / BOARD_WIDTH;
    //GLKMatrix4 projection = GLKMatrix4MakeFrustum(-2, 2, -aspectFrustum/2, aspectFrustum/2, 1, 1000);
    //GLKMatrix4 projection = GLKMatrix4MakePerspective(degToRad(90), aspect, 0.01f, 1000.0f);
    GLKMatrix4 projection = GLKMatrix4MakeOrtho(0, BOARD_WIDTH, 0, BOARD_HIEGHT, 1, 1000);
    glUniformMatrix4fv(_projectionUniform, 1, 0, projection.m);
    
    /*
    GLKMatrix4 identity = GLKMatrix4Identity;
    GLKMatrix4 lookAt = GLKMatrix4MakeLookAt(0, 0, 0, self.frame.size.width/2, self.frame.size.height/2, 1, 0, 1, 0);
    GLKMatrix4 plusTranslate = GLKMatrix4MakeTranslation(self.frame.size.width/2, self.frame.size.height/2, 0.0);
    GLKMatrix4 negTranslate = GLKMatrix4MakeTranslation(-self.frame.size.width/2, -self.frame.size.height/2, 0.0);
    GLKMatrix4 rotateY = GLKMatrix4MakeYRotation(degToRad(-90.0f));
    GLKMatrix4 rotateX = GLKMatrix4MakeXRotation(degToRad(45.0f));
    
    GLKMatrix4 result1 = GLKMatrix4Multiply(identity, lookAt);
    GLKMatrix4 result2 = GLKMatrix4Multiply(result1, plusTranslate);
    result1 = GLKMatrix4Multiply(result2, rotateY);
    result2 = GLKMatrix4Multiply(result1, rotateX);
    GLKMatrix4 modelView = GLKMatrix4Multiply(result2, negTranslate);
    glUniformMatrix4fv(_modelViewUniform, 1, 0, modelView.m);
    */
    _currentRotation += displayLink.duration * 90;
    
    // Sort out Model-View Matrix (For Orthographic View)
    GLKMatrix4 translation = GLKMatrix4MakeTranslation(0,0,-1);
    GLKMatrix4 rotation = GLKMatrix4MakeRotation(degToRad(0), 0, 0, 1);
    GLKMatrix4 modelView = GLKMatrix4Multiply(translation, rotation);
    
    glUniformMatrix4fv(_modelViewUniform, 1, 0, modelView.m);
    
    glViewport(0, 0, BOARD_WIDTH, BOARD_HIEGHT);// self.frame.size.width, self.frame.size.height);
    
    for (int i = 0; i < NUM_OF_PIECES; i++) {
        if (i != holdingPiece) {
            const Vertex NewPiece[] = {
                {{pieces[i].x_location + SIDE_HALF, pieces[i].y_location - SIDE_HALF, 0}, {0, 0, 0, 1}},
                {{pieces[i].x_location + SIDE_HALF, pieces[i].y_location + SIDE_HALF, 0}, {0, 0, 0, 1}},
                {{pieces[i].x_location - SIDE_HALF, pieces[i].y_location + SIDE_HALF, 0}, {0, 0, 0, 1}},
                {{pieces[i].x_location - SIDE_HALF, pieces[i].y_location - SIDE_HALF, 0}, {0, 0, 0, 1}}
            };
            
            GLuint vertexBuffer;
            glGenBuffers(1, &vertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(NewPiece), NewPiece, GL_STATIC_DRAW);
            
            glVertexAttribPointer(_positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
            glVertexAttribPointer(_colorSlot, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float)*3));
            glDrawElements(GL_TRIANGLES, sizeof(Indices)/sizeof(Indices[0]), GL_UNSIGNED_BYTE, 0);
        }
        else {
            const Vertex NewPiece[] = {
                {{SIDE_LENGTH, 0, 0}, {0.6, 0.6, 0.6, 0.5}},
                {{SIDE_LENGTH, SIDE_LENGTH, 0}, {0.6, 0.6, 0.6, 0.5}},
                {{0, SIDE_LENGTH, 0}, {0.6, 0.6, 0.6, 0.5}},
                {{0, 0, 0}, {0.6, 0.6, 0.6, 0.5}}
            };
            
            GLuint vertexBuffer;
            glGenBuffers(1, &vertexBuffer);
            glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
            glBufferData(GL_ARRAY_BUFFER, sizeof(NewPiece), NewPiece, GL_STATIC_DRAW);
            
            glVertexAttribPointer(_positionSlot, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
            glVertexAttribPointer(_colorSlot, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float)*3));
            glDrawElements(GL_TRIANGLES, sizeof(Indices)/sizeof(Indices[0]), GL_UNSIGNED_BYTE, 0);
        }
    }
    [_context presentRenderbuffer:GL_RENDERBUFFER];
}

- (void)setupDisplayLink {
    CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
    [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

/* "Main" for the frame */
- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Call all the openGl setup code
        [self setupLayer];
        [self setupContext];
        [self setupDepthBuffer];
        [self setupRenderBuffer];
        [self setupFrameBuffer];
        [self compileShaders];
        [self setupVBOs];
        [self setupDisplayLink];
        
        generatePieces(pieces);
    }
    return self;
}

@end
