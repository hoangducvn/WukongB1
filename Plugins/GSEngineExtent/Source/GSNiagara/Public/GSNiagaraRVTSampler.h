#pragma once
#include "CoreMinimal.h"
#include "NiagaraDataInterface.h"
#include "GSNiagaraRVTSampler.generated.h"

class URuntimeVirtualTexture;

UCLASS(Blueprintable, EditInlineNew)
class GSNIAGARA_API UGSNiagaraRVTSampler : public UNiagaraDataInterface {
    GENERATED_BODY()
public:

    DECLARE_NIAGARA_DI_PARAMETER();
    
    UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(AllowPrivateAccess=true))
    URuntimeVirtualTexture* VirtualTexture;
    
    UGSNiagaraRVTSampler();
    
};

