import React from 'react'
import { NEEDS_TO_BE_STOPPED, NOT_STARTED, STARTED, STOPPED } from '../constants'

const STATUS_STYLES = {
    [STARTED]: 'bg-green-400 drop-shadow-started',

    [STOPPED]: 'bg-red-400 drop-shadow-stopped',
    [NOT_STARTED]: 'bg-yellow-400 drop-shadow-not-started',
    [NEEDS_TO_BE_STOPPED]: 'bg-red-400 drop-shadow-stopped animate-blink ',
}

const Status = ({ status }) => {
    return (
        <div className="w-1/5 space-y-2 self-center flex flex-col items-center">
            <p className='text-xs'>Status</p>
            <div className={`w-10 h-10 md:w-16 md:h-16 rounded-full ${STATUS_STYLES[status]}`}></div>
        </div>
    )
}

export default Status