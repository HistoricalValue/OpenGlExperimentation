#ifndef __MY__DIRECT_3D_9__H__
#define __MY__DIRECT_3D_9__H__

#pragma warning( push, 0 )
#    include <d3d9.h>
#pragma warning( pop )

namespace my {

    class Direct3D9 {
    public:
                                                        Direct3D9 (void);
                                                        ~Direct3D9 (void);

        // Observers
        bool                                            IsValid (void) const;
    private:
        LPDIRECT3D9        direct3d9;
    };

} // namespace my

#endif // __MY__DIRECT_3D_9__H__
