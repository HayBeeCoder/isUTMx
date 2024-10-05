import React from 'react'

const Table = ({ test_info, dashboard_info }) => {
    return (
        <div className='h-full mx-auto max-w-[350px] w-full  overflow-auto    ' >
            <li className='font-extrabold bg-blue-950 text-white flex justify-stretch divide-x-2 py-3'>
                <h6 className='text-xs px-4 flex-1 md:w-16 text-right'>Force <br /> (N)</h6>
                <h6 className='text-xs px-4  flex-1 md:w-16 text-right'>displacement ({dashboard_info["text_type"]?.toLowerCase() == "torsion" ? "°" : "mm"}) </h6>
            </li>
            <ul className='md:h-[70vh]  overflow-y-scroll relative  scrollbar-thin scroll-shadows scrollbar-thumb-gray-700  scrollbar-track-gray-100 pb-28-'>
                {
                    (test_info)?.map((item, index) => <li className={`flex   ${index % 2 ? "" : "bg-gray-200"}`}>
                        <h6 className='text-sm px-4 w-32 md:w-16 py-3 flex-1 text-right border-r-[1px] border-gray-400'>{item?.force_in_newton}</h6>
                        <h6 className='text-sm px-4 w-32 md:w-16 py-3 flex-1 text-right' >{item?.displacement_in_mm} </h6>
                    </li>)
                }


            </ul>




        </div>
    )
}

export default Table