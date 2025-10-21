#include "RenderTargetReader.h"
#include "Kismet/KismetRenderingLibrary.h"

FLinearColor URenderTargetReader::ReadCenterColor(UTextureRenderTarget2D* RenderTarget)
{
    if (!RenderTarget) return FLinearColor::Black;

    int32 Width = RenderTarget->SizeX;
    int32 Height = RenderTarget->SizeY;
    int32 CenterX = Width / 2;
    int32 CenterY = Height / 2;

    return UKismetRenderingLibrary::ReadRenderTargetRawPixel(this, RenderTarget, CenterX, CenterY);

}
