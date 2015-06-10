//
//  Shader.fsh
//  Giguesaur
//
//  Created by Local Ash on 6/10/15.
//  Copyright (c) 2015 Local Ash. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
