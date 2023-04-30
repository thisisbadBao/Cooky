sound = {}

function sound:AddSFX(audioName, fileName)
    AddSoundEffect(audioName, fileName)
end

function sound:AddStream(audioName, fileName)
    AddSoundStream(audioName, fileName)
end

function sound:Play(audioName, volume)
    if volume == nil then
        PlaySoundEffect(audioName, 1.0)
    else
        PlaySoundEffect(audioName, volume)
    end
end

function sound:Stop(audioName)
    StopSoundEffect(audioName)
end

function sound:PlayStream(audioName, volume)
    if volume == nil then
        PlayStream(audioName, 1.0)
    else
        PlayStream(audioName, volume)
    end
end

function sound:StopStream(audioName)
    StopStream(audioName)
end
