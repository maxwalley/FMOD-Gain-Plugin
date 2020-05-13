//
//  Distortion.cpp
//  Fmod Plugin
//
//  Created by Max Walley on 30/04/2020.
//  Copyright © 2020 Max Walley. All rights reserved.
//

#include "fmod.hpp"
#include "fmod_errors.h"
#include <stdio.h>
#include <cstring>
#include <math.h>

void errorCheck(FMOD_RESULT result);

extern "C"
{
    F_EXPORT FMOD_DSP_DESCRIPTION* F_CALL FMODGetDSPDescription();
}

FMOD_RESULT F_CALLBACK create(FMOD_DSP_STATE* state);
FMOD_RESULT F_CALLBACK release(FMOD_DSP_STATE* state);
FMOD_RESULT F_CALLBACK reset(FMOD_DSP_STATE* state);
FMOD_RESULT F_CALLBACK read(FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, unsigned int length, int inchannels, int *outchannels);
FMOD_RESULT F_CALLBACK setFloat(FMOD_DSP_STATE* state, int index, float value);
FMOD_RESULT F_CALLBACK getFloat(FMOD_DSP_STATE* state, int index, float* value, char* valuestr);
FMOD_RESULT F_CALLBACK shouldIProcess(FMOD_DSP_STATE *dsp_state, FMOD_BOOL inputsidle, unsigned int length, FMOD_CHANNELMASK inmask, int inchannels, FMOD_SPEAKERMODE speakermode);

//Parameter definitions here
FMOD_DSP_PARAMETER_DESC gain;

//List of pointers to parameters
FMOD_DSP_PARAMETER_DESC* params[1]
{
    &gain
};

FMOD_DSP_DESCRIPTION pluginDesc =
{
    FMOD_PLUGIN_SDK_VERSION,
    "MaxWalley Gain",
    1,
    1,
    1,
    create,
    release,
    reset,
    read,
    0,
    0,
    1,
    params,
    setFloat,
    0,
    0,
    0,
    getFloat,
    0,
    0,
    0,
    shouldIProcess,
    0,
    0,
    0,
    0
};

void errorCheck(FMOD_RESULT result)
{
    if(result != FMOD_OK)
    {
        printf("FMOD Error %d: %s\n", result, FMOD_ErrorString(result));
    }
}

extern "C"
{
    F_EXPORT FMOD_DSP_DESCRIPTION* F_CALL FMODGetDSPDescription()
    {
        FMOD_DSP_INIT_PARAMDESC_FLOAT(gain, "Gain", "dB", "Level Gain", -80.0, 10.0, 0.0);
        return &pluginDesc;
    }
}

class pluginData
{
public:
    pluginData();
    
    void setGain(float newGain);
    float getGain() const;
    
private:
    float pGain;
};

pluginData::pluginData() : pGain(1.0)
{
}

void pluginData::setGain(float newGain)
{
    pGain = newGain;
}

float pluginData::getGain() const
{
    return pGain;
}



//Referenced from FMOD gain example
FMOD_RESULT F_CALLBACK create(FMOD_DSP_STATE* state)
{
    //Allocating memory for the plugin
    state->plugindata = (pluginData*) FMOD_DSP_ALLOC(state, sizeof(pluginData));
    
    //Checking that memory has been correctly allocated
    if(state->plugindata == nullptr)
    {
        return FMOD_ERR_MEMORY;
    }
    
    return FMOD_OK;
}

//Referenced from FMOD gain example
FMOD_RESULT F_CALLBACK release(FMOD_DSP_STATE* state)
{
    //Deallocating the memory for the plugin
    pluginData* data = (pluginData*)state->plugindata;
    FMOD_DSP_FREE(state, data);
    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK reset(FMOD_DSP_STATE* state)
{
    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK read(FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, unsigned int length, int inchannels, int *outchannels)
{
    pluginData* data = (pluginData*)dsp_state->plugindata;
    
    
    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < inchannels; j++)
        {
            *outbuffer++ = *inbuffer++ * data->getGain();
        }
    }
    
    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK setFloat(FMOD_DSP_STATE* state, int index, float value)
{
    if(index == 0)
    {
        pluginData* data = (pluginData*)state->plugindata;
        data->setGain(pow(10, (value/20)));
        return FMOD_OK;
    }
    else
    {
        return FMOD_ERR_INVALID_PARAM;
    }
}

FMOD_RESULT F_CALLBACK getFloat(FMOD_DSP_STATE* state, int index, float* value, char* valuestr)
{
    pluginData* data = (pluginData*)state->plugindata;
    *value = data->getGain();
    sprintf(valuestr, "%f", data->getGain());
    return FMOD_OK;
}

FMOD_RESULT F_CALLBACK shouldIProcess(FMOD_DSP_STATE *dsp_state, FMOD_BOOL inputsidle, unsigned int length, FMOD_CHANNELMASK inmask, int inchannels, FMOD_SPEAKERMODE speakermode)
{
    if(inputsidle == true)
    {
        return FMOD_ERR_DSP_DONTPROCESS;
    }
    return FMOD_OK;
}
