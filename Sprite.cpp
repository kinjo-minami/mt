#include"Sprite.h"
#include<d3dx12.h>
using namespace DirectX;
using namespace Microsoft::WRL;
void Sprite::initialize(SpriteCommon* spriteCommon, UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX , bool isFlipY)
{
    HRESULT result = S_FALSE;

 
    // テクスチャ番号をコピー
    texNumber_ = texNumber;

    // アンカーポイントをコピー
    anchorpoint_ = anchorpoint;

    // 反転フラグをコピー
    isFlipX_ = isFlipX;
    isFlipY_ = isFlipY;

    SspriteCommon = spriteCommon;

    // 頂点データ
    VertexPosUv vertices[4];

    // 指定番号の画像が読み込み済みなら
    if (SspriteCommon->GetTexBuff(texNumber_)) {
        // テクスチャ情報取得
        D3D12_RESOURCE_DESC resDesc = SspriteCommon->GetTexBuff(texNumber_)->GetDesc();

        // スプライトの大きさを画像の解像度に合わせる
        size_ = { (float)resDesc.Width, (float)resDesc.Height };
        texSize_ = { (float)resDesc.Width, (float)resDesc.Height };
    }

    // 頂点バッファ生成
    result = SspriteCommon->GetDevice()->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices)),
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vertBuff_));

    // 頂点バッファデータ転送
    TransferVertexBuffer();

    // 頂点バッファビューの作成
    vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
    vbView_.SizeInBytes = sizeof(vertices);
    vbView_.StrideInBytes = sizeof(vertices[0]);

    // 定数バッファの生成
    result = SspriteCommon->GetDevice()->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xff) & ~0xff),
        D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
        IID_PPV_ARGS(&constBuff_));

    // 定数バッファにデータ転送
    ConstBufferData* constMap = nullptr;
    result = constBuff_->Map(0, nullptr, (void**)&constMap);
    constMap->color = XMFLOAT4(1, 1, 1, 1); // 色指定（RGBA）
    constMap->mat = spriteCommon->GetMatProjection();
    constBuff_->Unmap(0, nullptr);

}

void Sprite::TransferVertexBuffer()
{
    HRESULT result = S_FALSE;

    // 頂点データ
    VertexPosUv vertices[] = {
        //     u     v
        {{}, {0.0f, 1.0f}}, // 左下
        {{}, {0.0f, 0.0f}}, // 左上
        {{}, {1.0f, 1.0f}}, // 右下
        {{}, {1.0f, 0.0f}}, // 右上
    };

    // 左下、左上、右下、右上
    enum { LB, LT, RB, RT };

    float left = (0.0f - anchorpoint_.x) * size_.x;
    float right = (1.0f - anchorpoint_.x) * size_.x;
    float top = (0.0f - anchorpoint_.y) * size_.y;
    float bottom = (1.0f - anchorpoint_.y) * size_.y;

    if (isFlipX_)
    {// 左右入れ替え
        left = -left;
        right = -right;
    }

    if (isFlipY_)
    {// 左右入れ替え
        top = -top;
        bottom = -bottom;
    }

    vertices[LB].pos = { left, bottom,  0.0f }; // 左下
    vertices[LT].pos = { left, top,     0.0f }; // 左上
    vertices[RB].pos = { right, bottom, 0.0f }; // 右下
    vertices[RT].pos = { right, top,    0.0f }; // 右上

    // 指定番号の画像が読み込み済みなら
    if (SspriteCommon->GetTexBuff(texNumber_)) {
        // テクスチャ情報取得
        D3D12_RESOURCE_DESC resDesc = SspriteCommon->GetTexBuff(texNumber_)->GetDesc();

        float tex_left = texLeftTop_.x / resDesc.Width;
        float tex_right = (texLeftTop_.x + texSize_.x) / resDesc.Width;
        float tex_top = texLeftTop_.y / resDesc.Height;
        float tex_bottom = (texLeftTop_.y + texSize_.y) / resDesc.Height;

        vertices[LB].uv = { tex_left,   tex_bottom }; // 左下
        vertices[LT].uv = { tex_left,   tex_top }; // 左上
        vertices[RB].uv = { tex_right,  tex_bottom }; // 右下
        vertices[RT].uv = { tex_right,  tex_top }; // 右上
    }

    // 頂点バッファへのデータ転送
    VertexPosUv* vertMap = nullptr;
    result = vertBuff_->Map(0, nullptr, (void**)&vertMap);
    memcpy(vertMap, vertices, sizeof(vertices));
    vertBuff_->Unmap(0, nullptr);
}
